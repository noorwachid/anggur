#include <glad/glad.h>
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
    firstChar = 'A';
    lastChar = 'Z';
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

    // TODO: Check this boy, if it really the right way to do.
    int maxBufferSize;
    glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxBufferSize);
    int inSquareTermSize = Math::Sqrt(maxBufferSize / 8);
    int bitmapWidth  = inSquareTermSize;
    int bitmapHeight = inSquareTermSize;

    normalized.Set(1.0 / bitmapWidth, 1.0 / bitmapHeight);
    int bitmapSize = bitmapWidth * bitmapHeight;
    uint8_t* bitmap = new uint8_t[bitmapSize];

    // Zeroing to avoid undefined behaviour
    for (int i = 0; i < bitmapSize; ++i)
        bitmap[i] = 0;

    int y = 0;
    int x = 0;
    int xPadding = 0;

    for (int c = firstChar; c <= lastChar; ++c)
    {
        int ax, lsb;
        int x0, y0, x1, y1;
        stbtt_GetCodepointHMetrics(infoData, c, &ax, &lsb);
        stbtt_GetCodepointBitmapBox(infoData, c, scale, scale, &x0, &y0, &x1, &y1);

        ax *= scale;

        int w = x1 - x0;
        int h = y1 - y0;

        y += ascent + y0;

        CharRect& cr = GetCharRect(c);
        cr.x = x0 + x;
        cr.y = y;
        cr.width = w;
        cr.height = h;

        x += ax + xPadding;

        cr.texOffsetX = cr.x * normalized.x;
        cr.texClipX = cr.width * normalized.x;
        cr.ratio = cr.width * normalized.y;

        int byteOffset = cr.x + (cr.y * bitmapWidth);

        stbtt_MakeCodepointBitmap(infoData, bitmap + byteOffset,
                                  cr.width, cr.height,
                                  bitmapWidth, scale, scale, c);
    }

    // Flip bitmap vertically
    int halfBitmapHeight = bitmapHeight / 2;
    for (int i = 0, ii = bitmapHeight - 1; i < halfBitmapHeight; ++i, --ii)
    {
        for (int j = 0; j < bitmapWidth; ++j)
        {
            int k = bitmapWidth * i + j;
            int l = bitmapWidth * ii + j;
            uint8_t tmp = bitmap[l];
            bitmap[l] = bitmap[k];
            bitmap[k] = tmp;
        }
    }
    texture.LoadBitmap(bitmap, bitmapWidth, bitmapHeight, 1);
}

}
