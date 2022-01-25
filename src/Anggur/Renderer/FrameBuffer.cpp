#include "FrameBuffer.h"
#include <Anggur/Helper/Log.h>
#include <glad/glad.h>

namespace Anggur
{

	FrameBuffer::FrameBuffer()
	{
	}

	FrameBuffer::FrameBuffer(uint width, uint height)
	{
		Recreate(width, height);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &_id);
		glDeleteTextures(1, &_colorAttachment);
	}

	void FrameBuffer::Recreate(uint width, uint height)
	{
		this->_width = width;
		this->_height = height;

		if (_id)
		{
			glDeleteFramebuffers(1, &_id);
			glDeleteTextures(1, &_colorAttachment);
		}

		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		glGenTextures(1, &_colorAttachment);
		glBindTexture(GL_TEXTURE_2D, _colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			_colorAttachment, 0);

		ANGGUR_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
			GL_FRAMEBUFFER_COMPLETE,
			"Framebuffer is incomplete!");

		glViewport(0, 0, width, height);
	}

	Texture FrameBuffer::ToTexture()
	{
		return Texture(_colorAttachment, _width, _height);
	}

	void FrameBuffer::BindDefault()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

} // namespace Anggur
