#pragma once

#include <inttypes.h>
#include <cstddef>

using std::size_t;

namespace Anggur
{
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
		uint32_t _id;
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
		uint32_t _id;
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
		uint32_t _id;
	};
}
