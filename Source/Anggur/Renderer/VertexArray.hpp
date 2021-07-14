#pragma once

#include <Anggur/Helper/Primitive.hpp>

namespace Anggur {

class IndexBuffer
{
public:
    void Create();
    void Destroy();
    void Bind();
    void Unbind();
    void SetCapacity(usize size);
    void SetData(usize size, uint* data);

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
    void SetCapacity(usize size);
    void SetData(usize size, float* data);

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

    void SetAttributePtr(usize index, usize size, usize stride, void* offset);

private:
    unsigned int id;
};

}
