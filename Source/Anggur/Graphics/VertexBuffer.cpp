#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Function.h"

namespace Anggur
{
	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &id);
		Bind();
	}

	VertexBuffer::~VertexBuffer()
	{
		if (id > 0)
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

	void VertexBuffer::SetCapacity(usize byteSize)
	{
		glBufferData(GL_ARRAY_BUFFER, byteSize, nullptr, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::SetData(usize byteSize, void* byteData)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, byteSize, byteData);
	}

	IndexBuffer::IndexBuffer()
	{
		glGenBuffers(1, &id);
		Bind();
	}

	IndexBuffer::~IndexBuffer()
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

	void IndexBuffer::SetCapacity(usize byteSize)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, nullptr, GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::SetData(usize byteSize, void* byteData)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, byteSize, byteData);
	}

	usize GetVertexDataTypeByteSize(VertexDataType type)
	{
		switch (type) 
		{
			case VertexDataType::Float:
				return 4;
		}
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &id);
		Bind();
	}

	VertexArray::~VertexArray()
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

	void VertexArray::SetAttribute(usize index, usize size, VertexDataType type, usize byteStride, usize byteOffset)
	{
		int glType = 0;

		switch (type) 
		{
			case VertexDataType::Float:
				glType = GL_FLOAT;
				break;
		}

		glVertexAttribPointer(index, size, glType, GL_FALSE, byteStride, (void*)byteOffset);
		glEnableVertexAttribArray(index);
	}

	void VertexArray::SetLayout(const std::vector<std::tuple<usize, VertexDataType>>& layout)
	{
		stride = 0;

		for (usize i = 0; i < layout.size(); ++i) 
		{
			auto [size, type] = layout[i];
			stride += size * GetVertexDataTypeByteSize(type);
		}

		usize offset = 0;

		for (usize i = 0; i < layout.size(); ++i) 
		{
			auto [size, type] = layout[i];
			usize byteCount = size * GetVertexDataTypeByteSize(type);

			SetAttribute(i, size, type, stride, offset);

			offset += byteCount;
		}
	}
}
