#pragma once
#include <Anggur/Helper/Primitive.hpp>
#include "Texture.hpp"

namespace Anggur {

class Framebuffer
{
public:
    Framebuffer();
    Framebuffer(uint width, uint height);
    ~Framebuffer();

    inline uint GetId() const { return id; }
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

