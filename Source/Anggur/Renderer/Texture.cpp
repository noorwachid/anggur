#include <glad/glad.h>
#include <stb_image.h>
#include <Anggur/Helper/Io.hpp>
#include <Anggur/Helper/Log.hpp>
#include "Texture.hpp"

namespace Anggur {

Texture::Texture()
{
    id = 0;
}

Texture::Texture(uint id, uint width, uint height)
{
    this->id = id;
    this->width = width;
    this->height = height;
}

Texture::Texture(const std::string& path, SamplerFilter filter)
{
    id = 0;
    Load(path, filter);
}

Texture::~Texture()
{
    Unload();
}

void Texture::LoadBitmap(uchar* data, uint width, uint height, uint channels, SamplerFilter filter)
{
    Unload();
    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    this->width = width;
    this->height = height;
    this->channels = channels;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(filter));

    if (data)
    {
        int iformat;
        int format;

        switch (channels)
        {
        case 1:
            iformat = GL_R8;
            format = GL_RED;
            break;
        case 4:
            iformat = GL_RGBA8;
            format = GL_RGBA;
            break;
        case 8:
        default:
            iformat = GL_RGB8;
            format = GL_RGB;
            break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        Anggur_Log("[Texture.LoadBitmap] Data is empty\n");
    }
}

void Texture::Load(const Image& image, SamplerFilter filter)
{
    width = image.GetWidth();
    height = image.GetHeight();
    channels = image.GetChannels();
    LoadBitmap(image.GetData(), width, height, channels, filter);
}

void Texture::Load(const std::string& path, SamplerFilter filter)
{
    Image image(path);
    Load(image, filter);
}

void Texture::Unload()
{
    if (id != 0)
        glDeleteTextures(1, &id);
}

void Texture::Bind(uint slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

bool operator==(const Texture& a, const Texture& b)
{
    return a.GetId() == b.GetId();
}

}
