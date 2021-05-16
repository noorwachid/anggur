#include <Anggur/Helper/Io.hpp>
#include <Anggur/Helper/Log.hpp>
#include "Font.hpp"

namespace Anggur {

Font::Font()
{
    initialize();
}

Font::~Font()
{
    if (mBuffer != nullptr)
    {
        delete[] mBuffer;
        mBuffer = nullptr;
    }
}

void Font::initialize()
{
    mFirstChar = 33;
    mLastChar = 126;

    mEmptyCharRect.x = 0;
    mEmptyCharRect.y = 0;
    mEmptyCharRect.width = 0;
    mEmptyCharRect.height = 0;
}

void Font::Load(const std::string& path, int height)
{
    mBuffer = Io::Load(path, mBufferSize);

    int result = stbtt_InitFont(&mInfoData, mBuffer, stbtt_GetFontOffsetForIndex(mBuffer, 0));
    Anggur_Assert(result, "[Renderer.Font] failed to load font");

    float scale = stbtt_ScaleForPixelHeight(&mInfoData, height);
    int ascent, decent;

    stbtt_GetFontVMetrics(&mInfoData, &ascent, &decent, 0);
    ascent *= scale;
    decent *= scale;
    height = (ascent - decent);

    //Io::WriteLine("Font.Load :: ascent: %d, decent: %d, height: %d", ascent, decent, height);

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

        GetCharRect(c).x = x0 + x;
        GetCharRect(c).y = y;
        GetCharRect(c).width  = w;
        GetCharRect(c).height = h;

        x += ax + 10;
        bitmapWidth += ax + 10;
    }

    int bitmapSize = bitmapWidth * bitmapHeight;
    uchar bitmap[bitmapSize];

    for (int i = 0; i < bitmapSize; ++i)
        bitmap[i] = 0;

    for (int c = mFirstChar; c <= mLastChar; ++c)
    {
        CharRect ci = GetCharRect(c);
        int byteOffset = ci.x + (ci.y * bitmapWidth);

        stbtt_MakeCodepointBitmap(&mInfoData, bitmap + byteOffset,
                                  ci.width, ci.height,
                                  bitmapWidth, scale, scale, c);
    }

    mTexture.LoadBitmap(bitmap, bitmapWidth, bitmapHeight, 1);
}

}
