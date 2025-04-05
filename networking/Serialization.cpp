#include "Serialization.h"
#include <cstdlib>
#include <cstring>
#include "ByteSwapper.h"

Buffer::Buffer(size_t size)
{
    m_Buffer = reinterpret_cast<char*>(std::malloc(size));

    if (!m_Buffer && size == 0)
    {
        throw "Cant allocate buffer";
    }

    m_MaxSize = size;
}

Buffer::~Buffer()
{
    std::free(m_Buffer);
}

Buffer::Buffer(const Buffer& rhs)
{
    m_Buffer = static_cast<char*>(std::malloc(rhs.m_MaxSize));

    if (!m_Buffer)
    {
        throw "Cant allocate buffer on copy";
    }

    memcpy(m_Buffer, rhs.m_Buffer, rhs.m_MaxSize);

    m_MaxSize = rhs.m_MaxSize;
    m_Read = rhs.m_Read;
    m_Write = rhs.m_Write;
}

Buffer::Buffer(Buffer&& rhs) noexcept
{
    m_Buffer = rhs.m_Buffer;
    m_MaxSize = rhs.m_MaxSize;
    m_Read = rhs.m_Read;
    m_Write = rhs.m_Write;
    rhs.m_Buffer = nullptr;
}

Buffer& Buffer::operator=(Buffer rhs)
{
    swap(*this, rhs);

    return *this;
}


void Buffer::write(void* data, size_t size)
{
    if (m_Write + size > m_MaxSize)
    {
        realloc(m_MaxSize + 10 * size);
    }
    memcpy(m_Buffer + m_Write, data, size);
    m_Write += size;
}

void Buffer::read(void* data, size_t size)
{
    if (m_Read + size < m_MaxSize)
    {
        memcpy(data, m_Buffer + m_Read, size);
        m_Read += size;
    }
}

void Buffer::realloc(size_t newSize)
{
    void* new_block = nullptr;
    new_block = std::realloc(m_Buffer, newSize);
    if (new_block)
    {
        m_Buffer = reinterpret_cast<char*>(new_block);
        m_MaxSize = newSize;
    }
    else
    {
        throw "cant realloc buffer";
    }
}
