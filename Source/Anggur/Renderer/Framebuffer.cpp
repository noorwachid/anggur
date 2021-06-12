#include <glad/glad.h>
#include <Anggur/Helper/Log.hpp>
#include "Framebuffer.hpp"

namespace Anggur {

Framebuffer::Framebuffer()
{
}

Framebuffer::Framebuffer(uint width, uint height)
{
    Recreate(width, height);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &id);
    glDeleteTextures(1, &colorAttachment);
}

void Framebuffer::Recreate(uint width, uint height)
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

    Anggur_Assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glViewport(0, 0, width, height);

}

Texture Framebuffer::ToTexture()
{
    return Texture(colorAttachment, width, height);
}

void Framebuffer::BindDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Gularen
