#include "Serialization.h"
#include "ByteSwapper.h"
#include <cstdlib>
#include <cstring>

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
    m_Buffer = reinterpret_cast<char*>(std::malloc(rhs.m_MaxSize));

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

}

void Buffer::read(void* data, size_t size)
{
    if (m_Read + size < m_MaxSize)
    {
        memcpy(data, m_Buffer + m_Read, size);
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

void* MemoryStream::getBuffer() const
{
    return m_Buffer;
}

size_t MemoryStream::getCapacity() const
{
    return m_Capacity;
}


InputMemoryStream::InputMemoryStream(void* buffer, size_t size)
{
    m_Buffer = reinterpret_cast<char*>(buffer);
    m_Capacity = size;
    m_Current = 0;
}

void InputMemoryStream::serialize(void* data, size_t size)
{
    read(data, size);
}

InputMemoryStream::~InputMemoryStream()
{
    //TODO(N1kla3): Maybe we should not destroy buffer since we dont create
}

void InputMemoryStream::read(void* data, size_t size)
{
    std::memcpy(data, m_Buffer + m_Current, size);
}


OutputMemoryStream::OutputMemoryStream(size_t size)
{
    m_Current = 0;
    m_Buffer = reinterpret_cast<char*>(std::malloc(size));
    if (m_Buffer)
    {
        m_Capacity = size;
    }
}

void OutputMemoryStream::serialize(void* data, size_t size)
{
    write(data, size);
}

OutputMemoryStream::~OutputMemoryStream()
{
    if (m_Capacity > 0)
    {
        std::free(m_Buffer);
    }
}

void OutputMemoryStream::write(void* data, size_t size)
{
    std::memcpy(m_Buffer + m_Current, data, size);
}


