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

	void VertexBuffer::SetCapacity(size_t byteSize)
	{
		glBufferData(GL_ARRAY_BUFFER, byteSize, nullptr, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::SetData(size_t byteSize, void* byteData)
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

	void IndexBuffer::SetCapacity(size_t byteSize)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, nullptr, GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::SetData(size_t byteSize, void* byteData)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, byteSize, byteData);
	}

	size_t GetVertexDataTypeByteSize(VertexDataType type)
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

	void VertexArray::SetAttribute(size_t index, size_t size, VertexDataType type, size_t byteStride, size_t byteOffset)
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

	void VertexArray::SetLayout(const std::vector<std::tuple<VertexDataType, size_t, std::string>>& layout)
	{
		stride = 0;

		for (size_t i = 0; i < layout.size(); ++i) 
		{
			auto [type, size, name] = layout[i];
			stride += size * GetVertexDataTypeByteSize(type);
		}

		size_t offset = 0;

		for (size_t i = 0; i < layout.size(); ++i) 
		{
			auto [type, size, name] = layout[i];
			size_t byteCount = size * GetVertexDataTypeByteSize(type);

			SetAttribute(i, size, type, stride, offset);

			offset += byteCount;
		}
	}
}
