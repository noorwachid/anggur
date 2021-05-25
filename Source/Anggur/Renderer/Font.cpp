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
    if (mBuffer != nullptr)
    {
        delete[] mBuffer;
        mBuffer = nullptr;
    }
}

void Font::Initialize()
{
    mFirstChar = 33;
    mLastChar = 126;
}

void Font::Load(const std::string& path, int height)
{
    mBuffer = Io::Load(path, mBufferSize);

    int result = stbtt_InitFont(&mInfoData, mBuffer, stbtt_GetFontOffsetForIndex(mBuffer, 0));
    Anggur_Assert(result, "[Renederer.Font] failed to load font");

    float scale = stbtt_ScaleForPixelHeight(&mInfoData, height);
    int ascent, decent;

    stbtt_GetFontVMetrics(&mInfoData, &ascent, &decent, 0);
    ascent *= scale;
    decent *= scale;
    height = (ascent - decent);

    int bitmapWidth  = 0;
    int bitmapHeight = height;

    int x = 0;
    int y = 0;

    for (int c = mFirstChar; c <= mLastChar; ++c)
    {
        int ax, lsb;
        int x0, y0, x1, y1;
        stbtt_GetCodepointHMetrics(&mInfoData, c, &ax, &lsb);
        stbtt_GetCodepointBitmapBox(&mInfoData, c, scale, scale, &x0, &y0, &x1, &y1);

        ax *= scale;

        int w = x1 - x0;
        int h = y1 - y0;

        y = ascent + y0;

        CharRect& cr = GetCharRect(c);
        cr.x = x0 + x;
        cr.y = y;
        cr.w = w;
        cr.h = h;

        x += ax + 10;
        bitmapWidth += ax + 10;
    }

    mNormalized.Set(1.0 / bitmapWidth, 1.0 / bitmapHeight);
    int bitmapSize = bitmapWidth * bitmapHeight;
    uchar bitmap[bitmapSize];
    uchar bitmapFlipped[bitmapSize];

    for (int i = 0; i < bitmapSize; ++i) // avoid undefined behaviour
    {
        bitmap[i] = 0;
        bitmapFlipped[i] = 0;
    }

    for (int c = mFirstChar; c <= mLastChar; ++c)
    {
        CharRect& cr = GetCharRect(c);
        cr.texOffsetX = cr.x * mNormalized.x;
        cr.texClipX = cr.w * mNormalized.x;
        cr.ratio = cr.w * mNormalized.y;

        int byteOffset = cr.x + (cr.y * bitmapWidth);

        stbtt_MakeCodepointBitmap(&mInfoData, bitmap + byteOffset,
                                  cr.w, cr.h,
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
    mTexture.LoadBitmap(bitmapFlipped, bitmapWidth, bitmapHeight, 1);
}

}
