#pragma once

#include <Anggur/Helper/Primitive.hpp>
#include <Anggur/Math/Vector.hpp>
#include "Texture.hpp"

struct stbtt_fontinfo;

namespace Anggur {

struct CharRect
{
    int X;
    int Y;
    int W;
    int H;

    float Ratio;

    float TexOffsetX;
    float TexClipX;

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
        if (c >= firstChar && c <= lastChar)
            return charRects[c - 32];
        return CharRect::Empty;
    }

    inline CharRect GetCharRect(int c) const
    {
        if (c >= firstChar && c <= lastChar)
            return charRects[c - 32];
        return CharRect::Empty;
    }

    inline Texture& GetTexture()
    {
        return texture;
    }

    inline Texture GetTexture() const
    {
        return texture;
    }

    inline Vector GetNormalized() const
    {
        return normalized;
    }

private:
    stbtt_fontinfo* infoData;
    uchar* buffer;
    size_t bufferSize;
    CharRect charRects[96];

    Texture texture;
    int firstChar;
    int lastChar;

    Vector normalized;
};

}
