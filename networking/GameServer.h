#pragma once

#include <memory>
#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>
#include <vector>
#include "PlayerRegistry.h"
#include "Serialization.h"

namespace oct
{
    class Connection
    {
    public:
        enum class ConnectionStatus : uint8
        {
            Connecting,
            Handshaking,
            Connected,
            PendingDisconnection,
            Disconnected,
            Invalid
        };

        Connection()
            : m_Buffer(128) {};
        void handleDisconnect();
        void handleConnect();

        void setStatus(ConnectionStatus status);
        ConnectionStatus getStatus() const noexcept;
        HSteamNetConnection getSteamConnection() const noexcept;

    private:
        ReplicationBuffer m_Buffer;
        HSteamNetConnection m_Connection = 0;
        ConnectionStatus m_Status = ConnectionStatus::Invalid;
    };

    class ConnectionHandle
    {
    public:
        bool isValid() const noexcept;
        void reset();

        ConnectionHandle() = default;
        ConnectionHandle(const ConnectionHandle& handle) = default;
        ConnectionHandle& operator=(const ConnectionHandle& handle) = default;
        ConnectionHandle(ConnectionHandle&& handle) = delete;
        ConnectionHandle& operator=(ConnectionHandle&& handle) = delete;

        explicit ConnectionHandle(const std::shared_ptr<Connection>& inConn);

    private:
        std::weak_ptr<Connection> m_Connection;
    };

    class GameServer
    {
    public:
        explicit GameServer(uint16 InPort);

        void update(float delta);
        void cleanupConnection(const ConnectionHandle& handle);
        void handleNewConnection(HSteamNetConnection conn);
        void closeConnection(const ConnectionHandle& handle);

        void onSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* inInfo);
        static void steamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);


    private:
        HSteamListenSocket m_ListenSocket;
        HSteamNetPollGroup m_PollGroup;
        ISteamNetworkingSockets* m_Interface;

        std::vector<std::shared_ptr<Connection>> m_Connections;
        PlayerRegistry m_PlayerRegistry;


        inline static GameServer* s_pCallbackInstance = nullptr;
    };
} // namespace oct
