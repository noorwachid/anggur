#pragma once

#include <stb_truetype.h>
#include <stb_image_write.h>
#include <Anggur/Helper/Primitive.hpp>
#include "Texture.hpp"

namespace Anggur {

struct CharRect
{
    int x;
    int y;
    int width;
    int height;
};

class Font
{
public:
    Font();
    ~Font();

    void initialize();
    void Load(const std::string& path, int height = 100);

    inline CharRect& GetCharRect(int c)
    {
        if (c >= mFirstChar && c <= mLastChar)
            return mCharRects[c - 32];
        return mEmptyCharRect;
    }

    inline CharRect GetCharRect(int c) const
    {
        if (c >= mFirstChar && c <= mLastChar)
            return mCharRects[c - 32];
        return mEmptyCharRect;
    }

    inline Texture& GetTexture()
    {
        return mTexture;
    }

    inline Texture GetTexture() const
    {
        return mTexture;
    }

private:
    stbtt_fontinfo mInfoData;
    uchar* mBuffer;
    size_t mBufferSize;
    CharRect mCharRects[96];
    CharRect mEmptyCharRect;

    Texture mTexture;
    int mFirstChar;
    int mLastChar;
};

}
