#pragma once

#include <stddef.h>
#include <cinttypes>

namespace Anggur {

class IndexBuffer
{
public:
    void Create();
    void Destroy();
    void Bind();
    void Unbind();
    void SetCapacity(size_t size);
    void SetData(size_t size, uint32_t* data);

private:
    unsigned int id;
};

class VertexBuffer
{
public:
    void Create();
    void Destroy();
    void Bind();
    void Unbind();
    void SetCapacity(size_t size);
    void SetData(size_t size, float* data);

private:
    unsigned int id;
};

class VertexArray
{
public:
    void Create();
    void Destroy();
    void Bind();
    void Unbind();

    void SetAttributePtr(size_t index, size_t size, size_t stride, void* offset);

private:
    unsigned int id;
};

}
