#include "Image.h"
#include <Anggur/Helper/IO.h>
#include <Anggur/Helper/Log.h>
#include <stb_image.h>

namespace Anggur
{

Image::Image() { data = nullptr; }

Image::Image(const string& path)
{
    data = nullptr;
    Load(path);
}

Image::~Image() { Unload(); }

void Image::Load(const string& path)
{
    Unload();

    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    ANGGUR_ASSERT(data, "[Renderer.Image] failed to load image\n");

    this->width = width;
    this->height = height;
    this->channels = channels;
}

void Image::Unload()
{
    if (data)
        stbi_image_free(data);
}

} // namespace Anggur
