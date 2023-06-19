#pragma once

#include <string>
#include <vector>

namespace Anggur {
	class IndexBuffer {
	public:
		IndexBuffer();
		~IndexBuffer();

		unsigned int getID() {
			return id;
		}

		void bind();
		void unbind();
		void setCapacity(size_t byteSize);
		void setData(size_t byteSize, void* byteData);

	private:
		unsigned int id = 0;
	};

	class VertexBuffer {
	public:
		VertexBuffer();
		~VertexBuffer();

		unsigned int getID() {
			return id;
		}

		void bind();
		void unbind();
		void setCapacity(size_t byteSize);
		void setData(size_t byteSize, void* byteData);

	private:
		unsigned int id;
	};

	enum class VertexDataType {
		float_,
	};

	size_t getVertexDataTypeByteSize(VertexDataType type);

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		unsigned int getID() {
			return id;
		}

		size_t getStride() {
			return stride;
		}

		void bind();
		void unbind();
		void setAttribute(size_t index, size_t size, VertexDataType type, size_t byteStride, size_t byteOffset);
		void setLayout(const std::vector<std::tuple<VertexDataType, size_t, std::string>>& layout);

	private:
		unsigned int id = 0;
		size_t stride = 0;
	};
}
