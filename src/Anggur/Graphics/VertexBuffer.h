#pragma once

#include <vector>
#include <string>

namespace Anggur
{
	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		unsigned int GetID()
		{
			return id;
		}

		void Bind();
		void Unbind();
		void SetCapacity(size_t byteSize);
		void SetData(size_t byteSize, void* byteData);

	private:
		unsigned int id = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		unsigned int GetID()
		{
			return id;
		}

		void Bind();
		void Unbind();
		void SetCapacity(size_t byteSize);
		void SetData(size_t byteSize, void* byteData);

	private:
		unsigned int id;
	};
	
	enum class VertexDataType 
	{
		Float,
	};

	size_t GetVertexDataTypeByteSize(VertexDataType type);

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		unsigned int GetID()
		{
			return id;
		}

		size_t GetStride() { return stride; }

		void Bind();
		void Unbind();
		void SetAttribute(size_t index, size_t size, VertexDataType type, size_t byteStride, size_t byteOffset);
		void SetLayout(const std::vector<std::tuple<VertexDataType, size_t, std::string>>& layout);

	private:
		unsigned int id = 0;
		size_t stride = 0;
	};
}
