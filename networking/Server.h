#pragma once

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <chrono>
#include <map>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <thread>

#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>

#include <windows.h>

inline SteamNetworkingMicroseconds g_logTimeZero;

static void DebugOutputDelegate(ESteamNetworkingSocketsDebugOutputType, const char* msg)
{
    printf("Steam %s", msg);
}

static void InitSteamDatagramConnectionSockets()
{
    SteamDatagramErrMsg err_msg;
    if (!GameNetworkingSockets_Init(nullptr, err_msg))
    {
        printf("%s", err_msg);
        assert(false);
    }

    g_logTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();

    SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, DebugOutputDelegate);
}

static void ShutdownSteamDatagramConnectionSockets()
{
    // Give connections time to finish up.  This is an application layer protocol
    // here, it's not TCP.  Note that if you have an application and you need to be
    // more sure about cleanup, you won't be able to do this.  You will need to send
    // a message and then either wait for the peer to close the connection, or
    // you can pool the connection to see if any reliable data is pending.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

#ifdef STEAMNETWORKINGSOCKETS_OPENSOURCE
    GameNetworkingSockets_Kill();
#else
    SteamDatagramClient_Kill();
#endif
}

// trim from start (in place)
static inline void ltrim(std::string& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int cha) { return !std::isspace(cha); }));
}

// trim from end (in place)
static inline void rtrim(std::string& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int cha) { return !std::isspace(cha); }).base(), str.end());
}


class ChatServer
{
public:
    void Run(uint16 nPort);
    void CloseServer();

private:
    HSteamListenSocket m_ListenSock;
    HSteamNetPollGroup m_PollGroup;
    ISteamNetworkingSockets* m_Interface;
    bool m_CloseServer = false;

    struct ClientData
    {
        std::string nick;
    };

    std::map<HSteamNetConnection, ClientData> m_MapClients;

    void SendStringToClient(HSteamNetConnection conn, const char* str);

    void SendStringToAllClients(const char* str, HSteamNetConnection except = k_HSteamNetConnection_Invalid);

    void PollIncomingMessages();

    void SetClientNick(HSteamNetConnection hConn, const char* nick);

    void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

    inline static ChatServer* s_pCallbackInstance = nullptr;
    static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
    {
        s_pCallbackInstance->OnSteamNetConnectionStatusChanged(pInfo);
    }

    void PollConnectionStateChanges();
};
