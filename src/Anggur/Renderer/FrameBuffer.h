#pragma once

#include "Texture.h"

namespace Anggur {

class FrameBuffer
{
public:
    FrameBuffer();
    FrameBuffer(uint width, uint height);
    ~FrameBuffer();

    inline uint GetID() const { return id; }
    inline uint GetColorAttachment() const { return colorAttachment; }
    inline uint GetWidth() const { return width; }
    inline uint GetHeight() const { return height; }

    void Bind();
    void Unbind();
    void Recreate(uint width, uint height);

    Texture ToTexture();

    static void BindDefault();

private:
    uint id;
    uint colorAttachment;
    uint width;
    uint height;
};

}

