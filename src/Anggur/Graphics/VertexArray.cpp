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

	void VertexBuffer::setCapacity(size_t size) {
		ANGGUR_GF(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, nullptr, GL_DYNAMIC_DRAW));
	}

	void VertexBuffer::setData(const std::vector<float>& data) {
		ANGGUR_GF(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * data.size(), data.data()));
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

	void IndexBuffer::setCapacity(size_t size) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * size, nullptr, GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::setData(const std::vector<uint32_t>& data) {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * data.size(), data.data());
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

	void VertexArray::setAttribute(size_t index, size_t stride, size_t size, size_t offset) {
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*) (sizeof(float) * offset));
		glEnableVertexAttribArray(index);
	}

	void VertexArray::drawUntil(size_t indexCount) {
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
}
