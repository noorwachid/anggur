#include <glad/glad.h>
#include <stb_truetype.h>
#include <stb_image_write.h>
#include <stb_rect_pack.h>
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
    lastChar = 127;
}

void Font::Load(const std::string& path, int height)
{
    buffer = IO::Load(path, bufferSize);

    if (!buffer)
        std::exit(123);

    if (!infoData)
        infoData = new stbtt_fontinfo;

    int result = stbtt_InitFont(infoData, buffer, stbtt_GetFontOffsetForIndex(buffer, 0));
    Anggur_Assert(result, "[Renderer.Font] failed to load font");

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
        bitmap[i] = 0x0;

    int gX = 0;
    int gY = 0;

    stbrp_context packingContext;
    std::vector<stbrp_rect> packingRects;
    std::vector<stbrp_node> packingNodes;

    for (int c = firstChar; c <= lastChar; ++c)
    {
        int ax, lsb;
        int x0, y0, x1, y1;
        stbtt_GetCodepointHMetrics(infoData, c, &ax, &lsb);
        stbtt_GetCodepointBitmapBox(infoData, c, scale, scale, &x0, &y0, &x1, &y1);

        stbrp_rect rect;
        rect.id = c;
        rect.x = 0;
        rect.y = 0;
        rect.w = x1 - x0;
        rect.h = y1 - y0;

        packingRects.push_back(rect);
    }

    packingNodes.resize(packingRects.size());
    stbrp_init_target(&packingContext, bitmapWidth, bitmapHeight, packingNodes.data(), packingNodes.size());
    stbrp_pack_rects(&packingContext, packingRects.data(), packingRects.size());

    for (stbrp_rect& rect: packingRects)
    {
        CharRect& cr = GetCharRect(rect.id);
        cr.x = rect.x;
        cr.y = rect.y;
        cr.w = rect.w;
        cr.h = rect.h;

        int byteOffset = cr.x + (cr.y * bitmapWidth);
        stbtt_MakeCodepointBitmap(infoData, bitmap + byteOffset, cr.w, cr.h, bitmapWidth, scale, scale, rect.id);
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
