#pragma once

#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>
#include <vector>

namespace oct
{
    class Connection
    {
    public:
        enum class ConnectionStatus : uint8
        {
            Connecting,
            Connected,
            PendingDisconnection,
            Disconnected,
            Invalid
        };

        Connection() = default;
        void handleDisconnect();
        void handleConnect();

        void setStatus(ConnectionStatus status);
        ConnectionStatus getStatus() const noexcept;
        HSteamNetConnection getSteamConnection() const noexcept;

    private:
        HSteamNetConnection m_Connection = 0;
        ConnectionStatus m_Status = ConnectionStatus::Invalid;
    };

    class GameServer
    {
    public:
        explicit GameServer(uint16 InPort);

        void update();
        void cleanupConnection(const Connection& connection);
        void setupConnection();
        void closeConnection();

        void onSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* inInfo);
        static void steamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);


    private:
        HSteamListenSocket m_ListenSocket;
        HSteamNetPollGroup m_PollGroup;
        ISteamNetworkingSockets* m_Interface;

        std::vector<Connection> m_Connections;


        inline static GameServer* s_pCallbackInstance = nullptr;
    };
} // namespace oct
