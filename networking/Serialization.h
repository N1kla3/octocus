#pragma once
#include <cstddef>
#include <utility>

class Buffer
{
public:
    explicit Buffer(size_t size);
    ~Buffer();

    Buffer(const Buffer& rhs);
    Buffer(Buffer&& rhs) noexcept;
    Buffer& operator=(Buffer rhs);
    friend void swap(Buffer& lhs, Buffer& rhs)
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

class MemoryStream
{
public:
    virtual void serialize(void* data, size_t size) = 0;
    void* getBuffer() const;
    size_t getCapacity() const;
    virtual ~MemoryStream(){}

protected:
    char* m_Buffer = nullptr;
    size_t m_Current = 0;
    size_t m_Capacity = 255;
};

class InputMemoryStream : public MemoryStream
{
public:
    InputMemoryStream(void* buffer, size_t size);
    virtual void serialize(void* data, size_t size) override;
    virtual ~InputMemoryStream();

private:
    void read(void* data, size_t size);
};

class OutputMemoryStream : public MemoryStream
{
    explicit OutputMemoryStream(size_t size);
    virtual void serialize(void* data, size_t size) override;
    virtual ~OutputMemoryStream();

private:
    void write(void* data, size_t size);
};
