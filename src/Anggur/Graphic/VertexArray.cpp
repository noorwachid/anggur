#include <Anggur/Graphic/Function.h>
#include <Anggur/Graphic/VertexArray.h>

namespace Anggur {

	void VertexBuffer::create() {
		glGenBuffers(1, &id);
	}

	void VertexBuffer::destroy() {
		glDeleteBuffers(1, &id);
	}

	void VertexBuffer::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void VertexBuffer::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::setCapacity(size_t size) {
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::setData(size_t size, float* data) {
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void IndexBuffer::create() {
		glGenBuffers(1, &_id);
	}

	void IndexBuffer::destroy() {
		glDeleteBuffers(1, &_id);
	}

	void IndexBuffer::bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}

	void IndexBuffer::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::setCapacity(size_t size) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::setData(size_t size, uint32_t* data) {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
	}

	void VertexArray::create() {
		glGenVertexArrays(1, &id);
	}

	void VertexArray::destroy() {
		glDeleteVertexArrays(1, &id);
	}

	void VertexArray::bind() {
		glBindVertexArray(id);
	}

	void VertexArray::unbind() {
		glBindVertexArray(0);
	}

	void VertexArray::setAttribute(size_t index, size_t size, size_t stride, void* offset) {
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(index);
	}
}
