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

    template< typename T, size_t Size>
    class ByteSwapper;

    template< typename T >
    class ByteSwapper<T, 2>
    {
    public:
        T swap(T inData) const
        {
            uint16_t result = byteSwap2(TypeAlliaser<T, uint16_t>(inData).get());
            return TypeAlliaser<uint16_t, T>(result).get();
        }
    };

    template< typename T >
    class ByteSwapper<T, 4>
    {
    public:
        T swap(T inData) const
        {
            uint32_t result = byteSwap4(TypeAlliaser<T, uint32_t>(inData).get());
            return TypeAlliaser<uint32_t, T>(result).get();
        }
    };

    template< typename T >
    class ByteSwapper<T, 8>
    {
    public:
        T swap(T inData) const
        {
            uint64_t result = byteSwap8(TypeAlliaser<T, uint64_t>(inData).get());
            return TypeAlliaser<uint64_t, T>(result).get();
        }
    };

    template< typename T >
    T byteSwap(T inData)
    {
        return ByteSwapper<T, sizeof(T)>().swap(inData);
    }
}
