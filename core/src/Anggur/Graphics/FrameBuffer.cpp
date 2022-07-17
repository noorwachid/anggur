#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/FrameBuffer.h>

namespace Anggur {
	FrameBuffer::FrameBuffer() {
		Initialize();
	}

	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height) {
		Initialize();
		Recreate(width, height);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &id);
		glDeleteTextures(1, &colorAttachment);
	}

	void FrameBuffer::Initialize() {
		texture = std::make_shared<Texture2D>();
	}

	void FrameBuffer::Recreate(uint32_t width, uint32_t height) {
		this->width = width;
		this->height = height;

		if (id) {
			glDeleteFramebuffers(1, &id);
			glDeleteTextures(1, &colorAttachment);
		}

		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		glGenTextures(1, &colorAttachment);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

		ANGGUR_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glViewport(0, 0, width, height);

		texture->id = colorAttachment;
		texture->width = width;
		texture->height = height;
	}

	void FrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void FrameBuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
