#pragma once

#include <string>

namespace Anggur {

class Image
{
public:
    Image();
    Image(const std::string& path);
    ~Image();

    inline uint GetWidth() const { return width; }
    inline uint GetHeight() const { return height; }
    inline uint GetChannels() const { return channels; }
    inline uint8_t* GetData() const { return data; }

    void Load(const std::string& path);
    void Unload();

private:
    uint width;
    uint height;
    uint channels;
    uint8_t* data;

};

}

