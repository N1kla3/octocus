#include "GameServer.h"
#include <cassert>
#include <cstdio>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>

oct::GameServer::GameServer(uint16 InPort)
{
    m_Interface = SteamNetworkingSockets();

    // Start listening
    SteamNetworkingIPAddr server_local_addr;
    server_local_addr.Clear();
    server_local_addr.m_port = InPort;
    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
               reinterpret_cast<void*>(steamNetConnectionStatusChangedCallback));
    m_ListenSocket = m_Interface->CreateListenSocketIP(server_local_addr, 1, &opt);
    if (m_ListenSocket == k_HSteamListenSocket_Invalid)
    {
        printf("Failed to listen on port %d", InPort);
    }
    m_PollGroup = m_Interface->CreatePollGroup();
    if (m_PollGroup == k_HSteamNetPollGroup_Invalid)
    {
        printf("Failed to listen on port %d", InPort);
    }
    printf("Server listening on port %d\n", InPort);
}

void oct::GameServer::onSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* inInfo)
{
    char temp[1024];

    // What's the state of the connection?
    switch (inInfo->m_info.m_eState)
    {
        case k_ESteamNetworkingConnectionState_None:
            // NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
            break;

        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        {
            // Ignore if they were not previously connected.  (If they disconnected
            // before we accepted the connection.)
            if (inInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
            {
                auto iter = m_Connections.begin();
                for (; iter != m_Connections.end(); iter++)
                {
                    if (iter->connection == inInfo->m_hConn)
                    {
                        break;
                    }
                }
                assert(iter != m_Connections.end());

                const char* psz_debug_log_action;
                if (inInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
                {
                    psz_debug_log_action = "problem detected locally";
                }
                else
                {
                    psz_debug_log_action = "closed by peer";
                }

                printf("Connection %s %s, reason %d: %s\n",
                       inInfo->m_info.m_szConnectionDescription,
                       psz_debug_log_action,
                       inInfo->m_info.m_eEndReason,
                       inInfo->m_info.m_szEndDebug);

                iter->handleDisconnect();
                m_Connections.erase(iter);
            }
            else
            {
                assert(inInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
            }

            // Clean up the connection.  This is important!
            // The connection is "closed" in the network sense, but
            // it has not been destroyed.  We must close it on our end, too
            // to finish up.  The reason information do not matter in this case,
            // and we cannot linger because it's already closed on the other end,
            // so we just pass 0's.
            m_Interface->CloseConnection(inInfo->m_hConn, 0, nullptr, false);
            break;
        }

        case k_ESteamNetworkingConnectionState_Connecting:
        {
            // This must be a new connection
            for (auto& conn: m_Connections)
            {
                assert(conn.connection != inInfo->m_hConn);
            }

            printf("Connection request from %s", inInfo->m_info.m_szConnectionDescription);

            // A client is attempting to connect
            // Try to accept the connection.
            if (m_Interface->AcceptConnection(inInfo->m_hConn) != k_EResultOK)
            {
                // This could fail.  If the remote host tried to connect, but then
                // disconnected, the connection may already be half closed.  Just
                // destroy whatever we have on our side.
                m_Interface->CloseConnection(inInfo->m_hConn, 0, nullptr, false);
                printf("Can't accept connection.  (It was already closed?)");
                break;
            }

            // Assign the poll group
            if (!m_Interface->SetConnectionPollGroup(inInfo->m_hConn, m_PollGroup))
            {
                m_Interface->CloseConnection(inInfo->m_hConn, 0, nullptr, false);
                printf("Failed to set poll group?");
                break;
            }

            Connection conn{};
            m_Connections.push_back(std::move(conn));
            m_Connections.back().handleConnect();
            break;
        }

        case k_ESteamNetworkingConnectionState_Connected:
            // We will get a callback immediately after accepting the connection.
            // Since we are the server, we can ignore this, it's not news to us.
            break;

        default:
            // Silences -Wswitch
            break;
    }
}


void oct::GameServer::steamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    s_pCallbackInstance->onSteamNetConnectionStatusChanged(pInfo);
}
