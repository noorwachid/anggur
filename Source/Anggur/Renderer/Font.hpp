#pragma once

#include <unordered_map>
#include <Anggur/Math/Vector.hpp>
#include "Texture.hpp"

struct stbtt_fontinfo;

namespace Anggur {

struct Glyph
{
    float x;
    float y;
    float w;
    float h;
    float scaleX;
    float scaleY;
    float ascent;
};

class Font
{
public:
    Font();
    ~Font();

    void Initialize();
    void Load(const std::string& path, int height = 100);

    inline Glyph& GetGlyph(int c)
    {
        return charRects[c];
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

    Texture texture;
    std::vector<int> ranges;
    std::unordered_map<int, Glyph> charRects;

    Vector normalized;
};

}
