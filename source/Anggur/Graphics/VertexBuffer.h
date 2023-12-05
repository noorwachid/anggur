#pragma once

#include <string>
#include <vector>

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
			return _id;
		}

		void Bind();

		void Unbind();

		void SetCapacity(size_t byteSize);

		void SetData(size_t byteSize, void* byteData);

	private:
		unsigned int _id;
	};

	enum class VertexDataType
	{
		Float,
	};

	size_t GetByteSize(VertexDataType type);

	class VertexArray
	{
	public:
		VertexArray();

		~VertexArray();

		unsigned int GetID()
		{
			return _id;
		}

		size_t GetStride()
		{
			return _stride;
		}

		void Bind();

		void Unbind();

		void SetAttribute(size_t index, size_t size, VertexDataType type, size_t byteStride, size_t byteOffset);

		void SetLayout(const std::vector<std::tuple<VertexDataType, size_t, std::string>>& layout);

	private:
		unsigned int _id = 0;

		size_t _stride = 0;
	};
}
