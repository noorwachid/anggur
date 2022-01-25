#include <glad/glad.h>
#include "VertexArray.h"

namespace Anggur
{

	void VertexBuffer::Create()
	{
		glGenBuffers(1, &_id);
	}

	void VertexBuffer::Destroy()
	{
		glDeleteBuffers(1, &_id);
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetCapacity(size_t size)
	{
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::SetData(size_t size, float* data)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void IndexBuffer::Create()
	{
		glGenBuffers(1, &_id);
	}

	void IndexBuffer::Destroy()
	{
		glDeleteBuffers(1, &_id);
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::SetCapacity(size_t size)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::SetData(size_t size, uint32_t* data)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
	}

	void VertexArray::Create()
	{
		glGenVertexArrays(1, &_id);
	}

	void VertexArray::Destroy()
	{
		glDeleteVertexArrays(1, &_id);
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(_id);
	}

	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::SetAttributePtr(size_t index, size_t size, size_t stride, void* offset)
	{
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(index);
	}

}
