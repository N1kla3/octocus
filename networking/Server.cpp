#include "Server.h"

void ChatServer::Run(uint16 nPort)
{
    // Select instance to use.  For now we'll always use the default.
    // But we could use SteamGameServerNetworkingSockets() on Steam.
    m_Interface = SteamNetworkingSockets();

    // Start listening
    SteamNetworkingIPAddr serverLocalAddr;
    serverLocalAddr.Clear();
    serverLocalAddr.m_port = nPort;
    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
               (void*) SteamNetConnectionStatusChangedCallback);
    m_ListenSock = m_Interface->CreateListenSocketIP(serverLocalAddr, 1, &opt);
    if (m_ListenSock == k_HSteamListenSocket_Invalid)
    {
        printf("Failed to listen on port %d", nPort);
    }
    m_PollGroup = m_Interface->CreatePollGroup();
    if (m_PollGroup == k_HSteamNetPollGroup_Invalid)
    {
        printf("Failed to listen on port %d", nPort);
    }
    printf("Server listening on port %d\n", nPort);

    while (!m_CloseServer)
    {
        PollIncomingMessages();
        PollConnectionStateChanges();
        // PollLocalUserInput();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Close all the connections
    printf("Closing connections...\n");
    for (auto it: m_MapClients)
    {
        // Send them one more goodbye message.  Note that we also have the
        // connection close reason as a place to send final data.  However,
        // that's usually best left for more diagnostic/debug text not actual
        // protocol strings.
        SendStringToClient(it.first, "Server is shutting down.  Goodbye.");

        // Close the connection.  We use "linger mode" to ask SteamNetworkingSockets
        // to flush this out and close gracefully.
        m_Interface->CloseConnection(it.first, 0, "Server Shutdown", true);
    }
    m_MapClients.clear();

    m_Interface->CloseListenSocket(m_ListenSock);
    m_ListenSock = k_HSteamListenSocket_Invalid;

    m_Interface->DestroyPollGroup(m_PollGroup);
    m_PollGroup = k_HSteamNetPollGroup_Invalid;
}

void ChatServer::CloseServer()
{
    m_CloseServer = true;
}

void ChatServer::SendStringToClient(HSteamNetConnection conn, const char* str)
{
    m_Interface->SendMessageToConnection(conn, str, (uint32) strlen(str), k_nSteamNetworkingSend_Reliable, nullptr);
}

void ChatServer::SendStringToAllClients(const char* str, HSteamNetConnection except)
{
    for (auto& c: m_MapClients)
    {
        if (c.first != except)
        {
            SendStringToClient(c.first, str);
        }
    }
}

void ChatServer::PollIncomingMessages()
{
    char temp[1024];

    while (!m_CloseServer)
    {
        ISteamNetworkingMessage* pIncomingMsg = nullptr;
        int numMsgs = m_Interface->ReceiveMessagesOnPollGroup(m_PollGroup, &pIncomingMsg, 1);
        if (numMsgs == 0)
        {
            break;
        }
        if (numMsgs < 0)
        {
            printf("Error checking for messages");
        }
        assert(numMsgs == 1 && pIncomingMsg);
        auto itClient = m_MapClients.find(pIncomingMsg->m_conn);
        assert(itClient != m_MapClients.end());

        // '\0'-terminate it to make it easier to parse
        std::string sCmd;
        sCmd.assign((const char*) pIncomingMsg->m_pData, pIncomingMsg->m_cbSize);
        const char* cmd = sCmd.c_str();

        // We don't need this anymore.
        pIncomingMsg->Release();

        // Check for known commands.  None of this example code is secure or robust.
        // Don't write a real server like this, please.

        if (strncmp(cmd, "/nick", 5) == 0)
        {
            const char* nick = cmd + 5;
            while (isspace(*nick))
                ++nick;

            // Let everybody else know they changed their name
            sprintf(temp, "%s shall henceforth be known as %s", itClient->second.nick.c_str(), nick);
            SendStringToAllClients(temp, itClient->first);

            // Respond to client
            sprintf(temp, "Ye shall henceforth be known as %s", nick);
            SendStringToClient(itClient->first, temp);

            // Actually change their name
            SetClientNick(itClient->first, nick);
            continue;
        }

        // Assume it's just a ordinary chat message, dispatch to everybody else
        sprintf(temp, "%s: %s", itClient->second.nick.c_str(), cmd);
        SendStringToAllClients(temp, itClient->first);
    }
}

void ChatServer::SetClientNick(HSteamNetConnection hConn, const char* nick)
{

    // Remember their nick
    m_MapClients[hConn].nick = nick;

    // Set the connection name, too, which is useful for debugging
    m_Interface->SetConnectionName(hConn, nick);
}

void ChatServer::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    char temp[1024];

    // What's the state of the connection?
    switch (pInfo->m_info.m_eState)
    {
        case k_ESteamNetworkingConnectionState_None:
            // NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
            break;

        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        {
            // Ignore if they were not previously connected.  (If they disconnected
            // before we accepted the connection.)
            if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
            {

                // Locate the client.  Note that it should have been found, because this
                // is the only codepath where we remove clients (except on shutdown),
                // and connection change callbacks are dispatched in queue order.
                auto itClient = m_MapClients.find(pInfo->m_hConn);
                assert(itClient != m_MapClients.end());

                // Select appropriate log messages
                const char* pszDebugLogAction;
                if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
                {
                    pszDebugLogAction = "problem detected locally";
                    sprintf(temp,
                            "Alas, %s hath fallen into shadow.  (%s)",
                            itClient->second.nick.c_str(),
                            pInfo->m_info.m_szEndDebug);
                }
                else
                {
                    // Note that here we could check the reason code to see if
                    // it was a "usual" connection or an "unusual" one.
                    pszDebugLogAction = "closed by peer";
                    sprintf(temp, "%s hath departed", itClient->second.nick.c_str());
                }

                // Spew something to our own log.  Note that because we put their nick
                // as the connection description, it will show up, along with their
                // transport-specific data (e.g. their IP address)
                printf("Connection %s %s, reason %d: %s\n",
                       pInfo->m_info.m_szConnectionDescription,
                       pszDebugLogAction,
                       pInfo->m_info.m_eEndReason,
                       pInfo->m_info.m_szEndDebug);

                m_MapClients.erase(itClient);

                // Send a message so everybody else knows what happened
                SendStringToAllClients(temp);
            }
            else
            {
                assert(pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
            }

            // Clean up the connection.  This is important!
            // The connection is "closed" in the network sense, but
            // it has not been destroyed.  We must close it on our end, too
            // to finish up.  The reason information do not matter in this case,
            // and we cannot linger because it's already closed on the other end,
            // so we just pass 0's.
            m_Interface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
            break;
        }

        case k_ESteamNetworkingConnectionState_Connecting:
        {
            // This must be a new connection
            assert(m_MapClients.find(pInfo->m_hConn) == m_MapClients.end());

            printf("Connection request from %s", pInfo->m_info.m_szConnectionDescription);

            // A client is attempting to connect
            // Try to accept the connection.
            if (m_Interface->AcceptConnection(pInfo->m_hConn) != k_EResultOK)
            {
                // This could fail.  If the remote host tried to connect, but then
                // disconnected, the connection may already be half closed.  Just
                // destroy whatever we have on our side.
                m_Interface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
                printf("Can't accept connection.  (It was already closed?)");
                break;
            }

            // Assign the poll group
            if (!m_Interface->SetConnectionPollGroup(pInfo->m_hConn, m_PollGroup))
            {
                m_Interface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
                printf("Failed to set poll group?");
                break;
            }

            // Generate a random nick.  A random temporary nick
            // is really dumb and not how you would write a real chat server.
            // You would want them to have some sort of signon message,
            // and you would keep their client in a state of limbo (connected,
            // but not logged on) until them.  I'm trying to keep this example
            // code really simple.
            char nick[64];
            sprintf(nick, "BraveWarrior%d", 10000 + (rand() % 100000));

            // Send them a welcome message
            sprintf(temp,
                    "Welcome, stranger.  Thou art known to us for now as '%s'; upon thine command '/nick' we shall "
                    "know thee otherwise.",
                    nick);
            SendStringToClient(pInfo->m_hConn, temp);

            // Also send them a list of everybody who is already connected
            if (m_MapClients.empty())
            {
                SendStringToClient(pInfo->m_hConn, "Thou art utterly alone.");
            }
            else
            {
                sprintf(temp, "%d companions greet you:", (int) m_MapClients.size());
                for (auto& c: m_MapClients)
                    SendStringToClient(pInfo->m_hConn, c.second.nick.c_str());
            }

            // Let everybody else know who they are for now
            sprintf(temp, "Hark!  A stranger hath joined this merry host.  For now we shall call them '%s'", nick);
            SendStringToAllClients(temp, pInfo->m_hConn);

            // Add them to the client list, using std::map wacky syntax
            m_MapClients[pInfo->m_hConn];
            SetClientNick(pInfo->m_hConn, nick);
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

void ChatServer::PollConnectionStateChanges()
{
    s_pCallbackInstance = this;
    m_Interface->RunCallbacks();
}
