#pragma once

#include <Anggur/Math/Vector.hpp>
#include "Texture.hpp"

struct stbtt_fontinfo;

namespace Anggur {

struct CharRect
{
    int x;
    int y;
    int width;
    int height;

    float ratio;

    float texOffsetX;
    float texClipX;

    static CharRect empty;
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
        return CharRect::empty;
    }

    inline CharRect GetCharRect(int c) const
    {
        if (c >= firstChar && c <= lastChar)
            return charRects[c - 32];
        return CharRect::empty;
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
    uint8_t* buffer;
    size_t bufferSize;
    CharRect charRects[96];

    Texture texture;
    int firstChar;
    int lastChar;

    Vector normalized;
};

}
