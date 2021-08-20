#include <stb_truetype.h>
#include <stb_image_write.h>
#include <Anggur/Helper/IO.hpp>
#include <Anggur/Helper/Log.hpp>
#include "Font.hpp"

namespace Anggur {

CharRect CharRect::empty = { 0, 0, 0, 0 };

Font::Font()
{
    Initialize();
}

Font::~Font()
{
    if (buffer != nullptr)
    {
        delete[] buffer;
        buffer = nullptr;
    }
    if (!infoData)
    {
        delete infoData;
        infoData = nullptr;
    }
}

void Font::Initialize()
{
    firstChar = 33;
    lastChar = 126;
}

void Font::Load(const std::string& path, int height)
{
    buffer = IO::Load(path, bufferSize);

    if (!buffer)
        std::exit(123);

    if (!infoData)
        infoData = new stbtt_fontinfo;

    int result = stbtt_InitFont(infoData, buffer, stbtt_GetFontOffsetForIndex(buffer, 0));
    Anggur_Assert(result, "[Renederer.Font] failed to load font");

    float scale = stbtt_ScaleForPixelHeight(infoData, height);
    int ascent, decent;

    stbtt_GetFontVMetrics(infoData, &ascent, &decent, 0);
    ascent *= scale;
    decent *= scale;
    height = (ascent - decent);

    int bitmapWidth  = 0;
    int bitmapHeight = height;

    int x = 0;
    int y = 0;

    for (int c = firstChar; c <= lastChar; ++c)
    {
        int ax, lsb;
        int x0, y0, x1, y1;
        stbtt_GetCodepointHMetrics(infoData, c, &ax, &lsb);
        stbtt_GetCodepointBitmapBox(infoData, c, scale, scale, &x0, &y0, &x1, &y1);

        ax *= scale;

        int w = x1 - x0;
        int h = y1 - y0;

        y = ascent + y0;

        CharRect& cr = GetCharRect(c);
        cr.x = x0 + x;
        cr.y = y;
        cr.width = w;
        cr.height = h;

        x += ax + 10;
        bitmapWidth += ax + 10;
    }

    normalized.Set(1.0 / bitmapWidth, 1.0 / bitmapHeight);
    int bitmapSize = bitmapWidth * bitmapHeight;
    uint8_t bitmap[bitmapSize];
    uint8_t bitmapFlipped[bitmapSize];

    for (int i = 0; i < bitmapSize; ++i) // avoid undefined behaviour
    {
        bitmap[i] = 0;
        bitmapFlipped[i] = 0;
    }

    for (int c = firstChar; c <= lastChar; ++c)
    {
        CharRect& cr = GetCharRect(c);
        cr.texOffsetX = cr.x * normalized.x;
        cr.texClipX = cr.width * normalized.x;
        cr.ratio = cr.width * normalized.y;

        int byteOffset = cr.x + (cr.y * bitmapWidth);

        stbtt_MakeCodepointBitmap(infoData, bitmap + byteOffset,
                                  cr.width, cr.height,
                                  bitmapWidth, scale, scale, c);
    }

    // flip bitmap vertically
    for (int i = 0, ii = bitmapHeight - 1; i < bitmapHeight; ++i, --ii)
    {
        for (int j = 0; j < bitmapWidth; ++j)
        {
            int k = bitmapWidth * i + j;
            int l = bitmapWidth * ii + j;
            bitmapFlipped[l] = bitmap[k];
        }
    }
    texture.LoadBitmap(bitmapFlipped, bitmapWidth, bitmapHeight, 1);
}

}
