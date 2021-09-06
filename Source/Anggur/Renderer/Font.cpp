#include <glad/glad.h>
#include <stb_truetype.h>
#include <stb_image_write.h>
#include <stb_rect_pack.h>
#include <Anggur/Helper/IO.hpp>
#include <Anggur/Helper/Log.hpp>
#include "Font.hpp"

namespace Anggur {

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
    // ASCII
    for (int c = 33; c < 127; ++c) ranges.push_back(c);

    // // Latin1
    // for (int c = 161; c < 256; ++c) ranges.push_back(c);
    //
    // // LatinExtendedA
    // for (int c = 256; c < 384; ++c) ranges.push_back(c);
    //
    // // LatinExtendedB
    // for (int c = 384; c < 592; ++c) ranges.push_back(c);
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
    float scalarPoint = height;

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


    for (int c: ranges)
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

        Glyph& glyph = GetGlyph(rect.id);
        glyph.w = rect.w * normalized.x;
        glyph.h = rect.h * normalized.y;
        glyph.scaleX = rect.w / scalarPoint;
        glyph.scaleY = rect.h / scalarPoint;
        glyph.ascent = (ascent + y0) / scalarPoint;

        packingRects.push_back(rect);
    }

    packingNodes.resize(packingRects.size());
    stbrp_init_target(&packingContext, bitmapWidth, bitmapHeight, packingNodes.data(), packingNodes.size());
    stbrp_pack_rects(&packingContext, packingRects.data(), packingRects.size());

    for (stbrp_rect& rect: packingRects)
    {
        Glyph& glyph = GetGlyph(rect.id);
        glyph.x = rect.x * normalized.x;
        glyph.y = rect.y * normalized.y;

        int byteOffset = rect.x + (rect.y * bitmapWidth);
        stbtt_MakeCodepointBitmap(infoData, bitmap + byteOffset, rect.w, rect.h, bitmapWidth, scale, scale, rect.id);
    }

    // Flip bitmap vertically
//    int halfBitmapHeight = bitmapHeight / 2;
//    for (int i = 0, ii = bitmapHeight - 1; i < halfBitmapHeight; ++i, --ii)
//    {
//        for (int j = 0; j < bitmapWidth; ++j)
//        {
//            int k = bitmapWidth * i + j;
//            int l = bitmapWidth * ii + j;
//            uint8_t tmp = bitmap[l];
//            bitmap[l] = bitmap[k];
//            bitmap[k] = tmp;
//        }
//    }
    texture.LoadBitmap(bitmap, bitmapWidth, bitmapHeight, 1);
}

}
