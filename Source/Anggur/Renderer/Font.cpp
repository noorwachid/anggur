#include <stb_truetype.h>
#include <stb_image_write.h>
#include <Anggur/Helper/Io.hpp>
#include <Anggur/Helper/Log.hpp>
#include "Font.hpp"

namespace Anggur {

CharRect CharRect::Empty = { 0, 0, 0, 0 };

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
    buffer = Io::Load(path, bufferSize);

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
        cr.X = x0 + x;
        cr.Y = y;
        cr.W = w;
        cr.H = h;

        x += ax + 10;
        bitmapWidth += ax + 10;
    }

    normalized.Set(1.0 / bitmapWidth, 1.0 / bitmapHeight);
    int bitmapSize = bitmapWidth * bitmapHeight;
    uchar bitmap[bitmapSize];
    uchar bitmapFlipped[bitmapSize];

    for (int i = 0; i < bitmapSize; ++i) // avoid undefined behaviour
    {
        bitmap[i] = 0;
        bitmapFlipped[i] = 0;
    }

    for (int c = firstChar; c <= lastChar; ++c)
    {
        CharRect& cr = GetCharRect(c);
        cr.TexOffsetX = cr.X * normalized.X;
        cr.TexClipX = cr.W * normalized.X;
        cr.Ratio = cr.W * normalized.Y;

        int byteOffset = cr.X + (cr.Y * bitmapWidth);

        stbtt_MakeCodepointBitmap(infoData, bitmap + byteOffset,
                                  cr.W, cr.H,
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
