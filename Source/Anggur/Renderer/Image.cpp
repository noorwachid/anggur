#include <stb_image.h>
#include <Anggur/Helper/Io.hpp>
#include <Anggur/Helper/Log.hpp>
#include "Image.hpp"

namespace Anggur {

Image::Image()
{
    mData = nullptr;
}

Image::Image(const std::string& path)
{
    mData = nullptr;
    Load(path);
}

Image::~Image()
{
    Unload();
}

void Image::Load(const std::string& path)
{
    Unload();

    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(1);
    mData = stbi_load(path.c_str(), &width, &height, &channels, 0);
    Anggur_Assert(mData, "[Renderer.Image] failed to load image\n");

    width = width;
    height = height;
    channels = channels;
}

void Image::Unload()
{
    if (mData)
        stbi_image_free(mData);
}


} // namespace Gularen
