#pragma once

#include <steam/steamtypes.h>
#include <vector>

namespace oct
{
    struct NewPlayerInfo
    {
        SteamIPAddress_t address;
    };

    class Player
    {
    public:
        static uint32 IdGenerator;

        Player();

    private:
        uint32 m_Id;
        SteamIPAddress_t m_IpAdress;
        // more data later
    };

    class PlayerRegistry
    {
    public:
        PlayerRegistry() = default;
        bool addPlayer(const NewPlayerInfo& playerInfo);

    private:
        std::vector<Player> m_Players;
    };
} // namespace oct
