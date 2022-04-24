#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

using std::size_t;

namespace Anggur {
	class IndexBuffer {
	public:
		IndexBuffer();
		~IndexBuffer();

		void bind();
		void unbind();
		void setCapacity(size_t size);
		void setData(const std::vector<uint32_t>& data);

	private:
		uint32_t id = 0;
	};

	class VertexBuffer {
	public:
		VertexBuffer();
		~VertexBuffer();

		void bind();
		void unbind();
		void setCapacity(size_t size);
		void setData(const std::vector<float>& data);

	private:
		uint32_t id;
	};

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		void bind();
		void unbind();
		void setAttribute(size_t index, size_t stride, size_t size, size_t offset = 0);
		void drawUntil(size_t indexCount);

	private:
		uint32_t id;
	};
}
