#pragma once
#include <cstddef>

class MemoryStream
{
public:
    virtual void serialize(void* data, size_t size) = 0;
    void* getBuffer() const;
    size_t getCapacity() const;
    virtual ~MemoryStream(){}

protected:
    void* m_Buffer = nullptr;
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
