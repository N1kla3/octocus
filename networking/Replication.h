#pragma once
#include <cstdint>
#include <map>

// SERVER
// entity -> update map
// write buffer
// send buffer
//
// receive buffer from client i
// update entity i
//
// CLIENT
// Receive server buffer
// update map data
// update entities -> read from map -> look for frame number
//
// Collect player data
// fill buffer -> send
//

namespace oct
{

    struct RepData
    {
    };

    class Replicator
    {
    private:
        std::map<uint32_t, RepData> m_Lookup;

    public:
        void addEntity(uint32_t entId);
    };

} // namespace oct
