#include "Serialization.h"
#include <cstdlib>

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
    m_Buffer = buffer;
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

}


OutputMemoryStream::OutputMemoryStream(size_t size)
{
    m_Current = 0;
    m_Buffer = std::malloc(size);
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

}


