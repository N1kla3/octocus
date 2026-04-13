#pragma once

#include <steam/steamtypes.h>

namespace oct
{
    class Player
    {
    public:
        void func();

    private:
        uint32 m_Id;
        SteamIPAddress_t m_IpAdress;
    };
} // namespace oct
