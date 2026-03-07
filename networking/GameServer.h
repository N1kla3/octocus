#pragma once

#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>
#include <vector>

namespace oct
{
    struct Connection
    {
        HSteamNetConnection connection;

    public:
        void handleDisconnect() {};
        void handleConnect() {};
    };

    class GameServer
    {
    public:
        explicit GameServer(uint16 InPort);

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
