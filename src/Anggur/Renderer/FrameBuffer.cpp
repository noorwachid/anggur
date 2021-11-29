#include "FrameBuffer.h"
#include <Anggur/Helper/Log.h>
#include <glad/glad.h>

namespace Anggur
{

FrameBuffer::FrameBuffer() {}

FrameBuffer::FrameBuffer(uint width, uint height) { Recreate(width, height); }

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &id);
    glDeleteTextures(1, &colorAttachment);
}

void FrameBuffer::Recreate(uint width, uint height)
{
    this->width = width;
    this->height = height;

    if (id)
    {
        glDeleteFramebuffers(1, &id);
        glDeleteTextures(1, &colorAttachment);
    }

    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    glGenTextures(1, &colorAttachment);
    glBindTexture(GL_TEXTURE_2D, colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           colorAttachment, 0);

    ANGGUR_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
                      GL_FRAMEBUFFER_COMPLETE,
                  "Framebuffer is incomplete!");

    glViewport(0, 0, width, height);
}

Texture FrameBuffer::ToTexture()
{
    return Texture(colorAttachment, width, height);
}

void FrameBuffer::BindDefault() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, id); }

void FrameBuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

} // namespace Anggur
