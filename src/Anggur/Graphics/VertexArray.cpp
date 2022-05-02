#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/VertexArray.h>

namespace Anggur {

	VertexBuffer::VertexBuffer() {
		ANGGUR_GF(glGenBuffers(1, &id));
		bind();
	}

	VertexBuffer::~VertexBuffer() {
		if (id > 0) {
			ANGGUR_GF(glDeleteBuffers(1, &id));
		}
	}

	void VertexBuffer::bind() {
		ANGGUR_GF(glBindBuffer(GL_ARRAY_BUFFER, id));
	}

	void VertexBuffer::unbind() {
		ANGGUR_GF(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::setCapacity(size_t byteSize) {
		ANGGUR_GF(glBufferData(GL_ARRAY_BUFFER, byteSize, nullptr, GL_DYNAMIC_DRAW));
	}

	void VertexBuffer::setData(size_t byteSize, void* byteData) {
		ANGGUR_GF(glBufferSubData(GL_ARRAY_BUFFER, 0, byteSize, byteData));
	}

	IndexBuffer::IndexBuffer() {
		glGenBuffers(1, &id);
		bind();
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &id);
	}

	void IndexBuffer::bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void IndexBuffer::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::setCapacity(size_t byteSize) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, nullptr, GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::setData(size_t byteSize, void* byteData) {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, byteSize, byteData);
	}

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &id);
		bind();
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &id);
	}

	void VertexArray::bind() {
		glBindVertexArray(id);
	}

	void VertexArray::unbind() {
		glBindVertexArray(0);
	}

	void VertexArray::setAttribute(size_t index, size_t size, int type, size_t byteStride, size_t byteOffset) {
		glVertexAttribPointer(index, size, type, GL_FALSE, byteStride, (void*) byteOffset);
		glEnableVertexAttribArray(index);
	}
}
