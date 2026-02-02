#include "Serialization.h"
#include <cassert>
#include <cstdlib>
#include <cstring>

ReplicationBuffer::ReplicationBuffer(size_t size)
{
    if (size == 0)
    {
        return;
    }

    m_Buffer = static_cast<char*>(std::malloc(size));

    if (!m_Buffer)
    {
        throw std::bad_alloc();
    }

    m_Reserved = size;
}

ReplicationBuffer::~ReplicationBuffer()
{
    std::free(m_Buffer);
}

ReplicationBuffer::ReplicationBuffer(const ReplicationBuffer& rhs)
{
    m_Buffer = static_cast<char*>(std::malloc(rhs.m_Reserved));

    if (!m_Buffer)
    {
        throw std::bad_alloc();
    }

    memcpy(m_Buffer, rhs.m_Buffer, rhs.m_Written);

    m_Reserved = rhs.m_Reserved;
    m_Head = rhs.m_Head;
    m_Written = rhs.m_Written;
}

ReplicationBuffer::ReplicationBuffer(ReplicationBuffer&& rhs) noexcept
{
    m_Buffer = rhs.m_Buffer;
    rhs.m_Buffer = nullptr;
    m_Reserved = rhs.m_Reserved;
    m_Head = rhs.m_Head;
    m_Written = rhs.m_Written;
}

ReplicationBuffer& ReplicationBuffer::operator=(const ReplicationBuffer& rhs)
{
    if (this == &rhs)
    {
        return *this;
    }
    std::free(m_Buffer);
    m_Buffer = static_cast<char*>(std::malloc(rhs.m_Reserved));

    if (!m_Buffer)
    {
        throw std::bad_alloc();
    }

    memcpy(m_Buffer, rhs.m_Buffer, rhs.m_Written);

    m_Reserved = rhs.m_Reserved;
    m_Head = rhs.m_Head;
    m_Written = rhs.m_Written;
    return *this;
}

ReplicationBuffer& ReplicationBuffer::operator=(ReplicationBuffer&& rhs) noexcept
{
    m_Buffer = rhs.m_Buffer;
    rhs.m_Buffer = nullptr;
    m_Reserved = rhs.m_Reserved;
    m_Head = rhs.m_Head;
    m_Written = rhs.m_Written;
    return *this;
}


void ReplicationBuffer::write(void* data, size_t size)
{
    if (m_Head + size > m_Reserved)
    {
        // TODO:
        realloc(m_Reserved + (10 * size));
    }
    memcpy(m_Buffer + m_Head, data, size);
    m_Head += size;
    m_Written = m_Head;
}

void ReplicationBuffer::read(void* data, size_t size)
{
    if (m_Head + size < m_Written)
    {
        memcpy(data, m_Buffer + m_Head, size);
        m_Head += size;
    }
    else
    {
        static_assert(true);
    }
}

void ReplicationBuffer::realloc(size_t newSize)
{
    void* new_block = nullptr;
    new_block = std::realloc(m_Buffer, newSize);
    if (new_block)
    {
        m_Buffer = static_cast<char*>(new_block);
        m_Reserved = newSize;
    }
    else
    {
        throw std::bad_alloc();
    }
}

void ReplicationBuffer::resetHead()
{
    m_Head = 0;
}

void ReplicationBuffer::resetWrittenData()
{
    m_Head = 0;
    m_Written = 0;
}
