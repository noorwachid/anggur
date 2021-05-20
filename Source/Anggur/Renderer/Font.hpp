#pragma once

#include <stb_truetype.h>
#include <stb_image_write.h>
#include <Anggur/Helper/Primitive.hpp>
#include <Anggur/Math/Vector.hpp>
#include "Texture.hpp"

namespace Anggur {

struct CharRect
{
    int x;
    int y;
    int w;
    int h;

    float ratio;

    float texOffsetX;
    float texClipX;

    static CharRect Empty;
};

class Font
{
public:
    Font();
    ~Font();

    void Initialize();
    void Load(const std::string& path, int height = 100);

    inline CharRect& GetCharRect(int c)
    {
        if (c >= mFirstChar && c <= mLastChar)
            return mCharRects[c - 32];
        return CharRect::Empty;
    }

    inline CharRect GetCharRect(int c) const
    {
        if (c >= mFirstChar && c <= mLastChar)
            return mCharRects[c - 32];
        return CharRect::Empty;
    }

    inline Texture& GetTexture()
    {
        return mTexture;
    }

    inline Texture GetTexture() const
    {
        return mTexture;
    }

    inline Vector GetNormalized() const
    {
        return mNormalized;
    }

private:
    stbtt_fontinfo mInfoData;
    uchar* mBuffer;
    size_t mBufferSize;
    CharRect mCharRects[96];

    Texture mTexture;
    int mFirstChar;
    int mLastChar;

    Vector mNormalized;
};

}
