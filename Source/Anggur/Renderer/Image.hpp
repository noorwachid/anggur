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

    inline uint GetWidth() const { return mWidth; }
    inline uint GetHeight() const { return mHeight; }
    inline uint GetChannels() const { return mChannels; }
    inline uchar* GetData() const { return mData; }

    void Load(const std::string& path);
    void Unload();

private:
    uint mWidth;
    uint mHeight;
    uint mChannels;
    uchar* mData;

};

}

