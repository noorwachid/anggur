#pragma once

#include <string>
#include <Anggur/Helper/Primitive.hpp>

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
    inline uchar* GetData() const { return data; }

    void Load(const std::string& path);
    void Unload();

private:
    uint width;
    uint height;
    uint channels;
    uchar* data;

};

}

