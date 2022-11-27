#pragma once

#include <cstdint>
#include <cstddef>

namespace Anggur {
	class IndexBuffer {
	public:
		IndexBuffer();
		~IndexBuffer();

		uint32_t getID() { return id; }

		void bind();
		void unbind();
		void setCapacity(size_t byteSize);
		void setData(size_t byteSize, void* byteData);

	private:
		uint32_t id = 0;
	};

	class VertexBuffer {
	public:
		VertexBuffer();
		~VertexBuffer();

		uint32_t getID() { return id; }

		void bind();
		void unbind();
		void setCapacity(size_t byteSize);
		void setData(size_t byteSize, void* byteData);

	private:
		uint32_t id;
	};

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		uint32_t getID() { return id; }

		void bind();
		void unbind();
		void setAttribute(size_t index, size_t size, int type, size_t byteStride, size_t byteOffset);

	private:
		uint32_t id;
	};
}
