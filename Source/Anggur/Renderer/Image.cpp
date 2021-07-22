#include <stb_image.h>
#include <Anggur/Helper/IO.hpp>
#include <Anggur/Helper/Log.hpp>
#include "Image.hpp"

namespace Anggur {

Image::Image()
{
    data = nullptr;
}

Image::Image(const std::string& path)
{
    data = nullptr;
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
    data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    Anggur_Assert(data, "[Renderer.Image] failed to load image\n");

    this->width = width;
    this->height = height;
    this->channels = channels;
}

void Image::Unload()
{
    if (data)
        stbi_image_free(data);
}


} // namespace Gularen
