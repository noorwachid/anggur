#pragma once
#include <Anggur/Helper/Primitive.hpp>
#include "Texture.hpp"

namespace Anggur {

class Framebuffer
{
public:
    Framebuffer(uint width, uint height);
    ~Framebuffer();

    inline uint GetId() const { return mId; }
    inline uint GetColorAttachment() const { return mColorAttachment; }
    inline uint GetWidth() const { return mWidth; }
    inline uint GetHeight() const { return mHeight; }

    void Bind();
    void Unbind();
    void Recreate(uint width, uint height);

    Texture ToTexture();

    static void BindDefault();

private:
    uint mId;
    uint mColorAttachment;
    uint mWidth;
    uint mHeight;
};

}

