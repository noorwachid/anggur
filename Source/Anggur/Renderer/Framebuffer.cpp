#include <glad/glad.h>
#include <Anggur/Helper/Log.hpp>
#include "Framebuffer.hpp"

namespace Anggur {

Framebuffer::Framebuffer(uint width, uint height)
{
    Recreate(width, height);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &mId);
    glDeleteTextures(1, &mColorAttachment);
}

void Framebuffer::Recreate(uint width, uint height)
{
    this->mWidth = width;
    this->mHeight = height;

    if (mId)
    {
        glDeleteFramebuffers(1, &mId);
        glDeleteTextures(1, &mColorAttachment);
    }

    glGenFramebuffers(1, &mId);
    glBindFramebuffer(GL_FRAMEBUFFER, mId);

    glGenTextures(1, &mColorAttachment);
    glBindTexture(GL_TEXTURE_2D, mColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

    Anggur_Assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glViewport(0, 0, width, height);

}

Texture Framebuffer::ToTexture()
{
    return Texture(mColorAttachment, mWidth, mHeight);
}

void Framebuffer::BindDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mId);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Gularen
