#include <glad/glad.h>
#include "VertexArray.hpp"

namespace Anggur {

void VertexBuffer::Create()
{
    glGenBuffers(1, &id);
}

void VertexBuffer::Destroy()
{
    glDeleteBuffers(1, &id);
}


void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void VertexBuffer::SetCapacity(usize size)
{
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

void VertexBuffer::SetData(usize size, float* data)
{
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void IndexBuffer::Create()
{
    glGenBuffers(1, &id);
}

void IndexBuffer::Destroy()
{
    glDeleteBuffers(1, &id);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SetCapacity(usize size)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

void IndexBuffer::SetData(usize size, uint* data)
{
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}

void VertexArray::Create()
{
    glGenVertexArrays(1, &id);
}

void VertexArray::Destroy()
{
    glDeleteVertexArrays(1, &id);
}

void VertexArray::Bind()
{
    glBindVertexArray(id);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::SetAttributePtr(usize index, usize size, usize stride, void* offset)
{
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
}

}
