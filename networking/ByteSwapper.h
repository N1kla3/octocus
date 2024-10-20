#pragma once
#include <cstdint>

namespace oct
{
    inline uint16_t byteSwap2(uint16_t inData)
    {
        return (inData >> 8) | (inData << 8);
    }

    inline uint32_t byteSwap4(uint32_t inData)
    {
        return  ((inData >> 24) & 0x000000ff) |
                ((inData >> 8)  & 0x0000ff00) |
                ((inData << 8)  & 0x00ff0000) |
                ((inData << 24) & 0xff000000);
    }

    inline uint64_t byteSwap8(uint64_t inData)
    {
        return  ((inData >> 56) & 0x00000000000000ff) |
                ((inData >> 40) & 0x000000000000ff00) |
                ((inData >> 24) & 0x0000000000ff0000) |
                ((inData >> 8)  & 0x00000000ff000000) |
                ((inData << 8)  & 0x000000ff00000000) |
                ((inData << 24) & 0x0000ff0000000000) |
                ((inData << 40) & 0x00ff000000000000) |
                ((inData << 56) & 0xff00000000000000);
    }

    template< typename From, typename To >
    class TypeAlliaser
    {
    public:
        explicit TypeAlliaser(From fromValue)
            : source(fromValue){}

        To& get() { return destination; }
        
        union
        {
            From source;
            To destination;
        };
    };
}
