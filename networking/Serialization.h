#pragma once
#include <bit>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include "ByteSwapper.h"

class Buffer
{
public:
    explicit Buffer(size_t size);
    ~Buffer();

    Buffer(const Buffer& rhs);
    Buffer(Buffer&& rhs) noexcept;
    Buffer& operator=(Buffer rhs);
    friend void swap(Buffer& lhs, Buffer& rhs) noexcept
    {
        using std::swap;

        swap(lhs.m_Buffer, rhs.m_Buffer);
        swap(lhs.m_MaxSize, rhs.m_MaxSize);
        swap(lhs.m_Write, rhs.m_Write);
        swap(lhs.m_Read, rhs.m_Read);
    }

    void write(void* data, size_t size);
    void read(void* data, size_t size);

private:
    char* m_Buffer = nullptr;
    size_t m_Read = 0;
    size_t m_Write = 0;
    size_t m_MaxSize = 0;

    void realloc(size_t newSize);
};

template<typename T>
concept NumberType = std::is_arithmetic_v<T> || std::is_enum_v<T>;

template<typename T>
class BufferStream
{
    friend T;

public:
    template<typename U>
    void serialize(U& data)
    {
        static_cast<T*>(this)->serialize_impl(data);
    }

private:
    BufferStream() = delete;
    explicit BufferStream(std::unique_ptr<Buffer>&& buffer)
        : m_Buffer(std::move(buffer))
    {
    }

protected:
    std::unique_ptr<Buffer> m_Buffer;
};

class ReadStream : public BufferStream<ReadStream>
{
public:
    friend class BufferStream;

    explicit ReadStream(std::unique_ptr<Buffer>&& buffer)
        : BufferStream<ReadStream>(std::move(buffer))
    {
    }

private:
    template<typename T>
    void serialize_impl(T& data)
    {
        read(data, sizeof(T));
    }
    template<typename T>
    void serialize_impl(std::vector<T>& data)
    {
        size_t size;
        read(size, sizeof(size_t));
        data.resize(size);
        for (size_t index = 0; index < size; index++)
        {
            read(data[index], sizeof(T));
        }
    }

private:
    template<NumberType T>
    void read(T& data, size_t size)
    {
        if constexpr (std::endian::native == std::endian::big)
        {
            data = oct::byteSwap(data);
        }
        m_Buffer->read(&data, size);
    }
};

class WriteStream : public BufferStream<WriteStream>
{
    friend class BufferStream;

public:
    explicit WriteStream(std::unique_ptr<Buffer>&& buffer)
        : BufferStream<WriteStream>(std::move(buffer))
    {
    }

private:
    template<typename T>
    void serialize_impl(T& data)
    {
        write(data, sizeof(T));
    }
    template<typename T>
    void serialize_impl(std::vector<T>& data)
    {
        size_t size = data.size();
        write(size, sizeof(size_t));

        for (T& val: data)
        {
            write(val, sizeof(T));
        }
    }

private:
    template<NumberType T>
    void write(T& data, size_t size)
    {
        m_Buffer->write(&data, size);
        if constexpr (std::endian::native == std::endian::big)
        {
            data = oct::byteSwap(data);
        }
    }
};
