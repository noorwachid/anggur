#pragma once
#include "Image.hpp"

namespace Anggur {

enum class SamplerFilter
{
    Nearest = 0x2600,
    Linear  = 0x2601
};

class Texture
{
public:
    Texture();
    Texture(uint id, uint width, uint height);
    Texture(const std::string& path, SamplerFilter filter = SamplerFilter::Linear);
    ~Texture();

    inline uint GetId() const { return id; }
    inline uint GetWidth() const { return width; }
    inline uint GetHeight() const { return height; }
    inline uint GetChannels() const { return channels; }

    void LoadBitmap(uchar* data, uint width, uint height, uint channels = 8, SamplerFilter filter = SamplerFilter::Linear);
    void Load(const Image& image, SamplerFilter filter = SamplerFilter::Linear);
    void Load(const std::string& path, SamplerFilter filter = SamplerFilter::Linear);
    void Unload();

    void Bind(uint slot = 0);

    friend bool operator==(const Texture& a, const Texture& b);

private:
    uint id;
    uint width;
    uint height;
    uint channels;

};

}
