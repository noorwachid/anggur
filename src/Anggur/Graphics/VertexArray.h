#pragma once

#include <cstdint>
#include <cstddef>

namespace Anggur 
{
	class IndexBuffer 
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		void Bind();
		void Unbind();
		void SetCapacity(size_t byteSize);
		void SetData(size_t byteSize, void* byteData);

	private:
		uint32_t id = 0;
	};

	class VertexBuffer 
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		void Bind();
		void Unbind();
		void SetCapacity(size_t byteSize);
		void SetData(size_t byteSize, void* byteData);

	private:
		uint32_t id;
	};

	class VertexArray 
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind();
		void Unbind();
		void SetAttribute(size_t index, size_t size, int type, size_t byteStride, size_t byteOffset);

	private:
		uint32_t id;
	};
}
