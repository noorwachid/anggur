#pragma once

#include <cstdint>
#include <cstddef>

using std::size_t;

namespace Anggur {
	class IndexBuffer {
	public:
		void create();
		void destroy();
		void bind();
		void unbind();
		void setCapacity(size_t size);
		void setData(size_t size, uint32_t* data);

	private:
		uint32_t _id;
	};

	class VertexBuffer {
	public:
		void create();
		void destroy();
		void bind();
		void unbind();
		void setCapacity(size_t size);
		void setData(size_t size, float* data);

	private:
		uint32_t id;
	};

	class VertexArray {
	public:
		void create();
		void destroy();
		void bind();
		void unbind();
		void setAttribute(size_t index, size_t size, size_t stride, void* offset);

	private:
		uint32_t id;
	};
}
