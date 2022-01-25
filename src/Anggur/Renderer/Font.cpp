#include "Font.h"
#include <Anggur/Helper/IO.h>
#include <Anggur/Helper/Log.h>
#include <glad/glad.h>
#include <stb_image_write.h>
#include <stb_rect_pack.h>
#include <stb_truetype.h>

namespace Anggur
{

	Font::Font()
	{
		Initialize();
	}

	Font::~Font()
	{
		if (!_info)
		{
			delete _info;
			_info = nullptr;
		}
	}

	void Font::Initialize()
	{
		// ASCII
		for (int c = 33; c < 127; ++c)
			_ranges.push_back(c);

		// // Latin1
		// for (int c = 161; c < 256; ++c) _ranges.push_back(c);
		//
		// // LatinExtendedA
		// for (int c = 256; c < 384; ++c) _ranges.push_back(c);
		//
		// // LatinExtendedB
		// for (int c = 384; c < 592; ++c) _ranges.push_back(c);
	}

	void Font::Load(const string& path, int sampleSize)
	{
		_buffer = IO::Load(path);

		if (_buffer.empty())
			std::exit(123);

		if (!_info)
			_info = new stbtt_fontinfo;

		this->_sampleSize = sampleSize;
		this->_invSampleSize = 1.f / sampleSize;

		int result = stbtt_InitFont(_info, _buffer.data(), 0);
		ANGGUR_ASSERT(result, "[Renderer.Font] failed to load font");

		float scale = stbtt_ScaleForPixelHeight(_info, sampleSize);
		int ascent, decent;
		stbtt_GetFontVMetrics(_info, &ascent, &decent, 0);
		ascent *= scale;
		decent *= scale;
		_lineHeight = (decent - ascent) * _invSampleSize;

		// TODO: Check this boy, if it really the right way to do.
		int maxBufferSize;
		glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxBufferSize);
		int inSquareTermSize = Math::Sqrt(maxBufferSize / 8);
		int bitmapWidth = inSquareTermSize;
		int bitmapHeight = inSquareTermSize;

		_normal.Set(1.0 / bitmapWidth, 1.0 / bitmapHeight);
		int bitmapSize = bitmapWidth * bitmapHeight;
		std::vector<uint8_t> bitmap(bitmapSize);

		int gX = 0;
		int gY = 0;

		stbrp_context packingContext;
		std::vector<stbrp_rect> packingRects;
		std::vector<stbrp_node> packingNodes;

		for (int c: _ranges)
		{
			int ax, lsb;
			int x0, y0, x1, y1;
			stbtt_GetCodepointHMetrics(_info, c, &ax, &lsb);
			stbtt_GetCodepointBitmapBox(_info, c, scale, scale, &x0, &y0, &x1, &y1);

			stbrp_rect rect;
			rect.id = c;
			rect.x = 0;
			rect.y = 0;
			rect.w = x1 - x0;
			rect.h = y1 - y0;

			Glyph& glyph = glyphs[rect.id];
			glyph.w = rect.w * _normal.x;
			glyph.h = rect.h * _normal.y;
			glyph.ascent = (ascent + y0) * _invSampleSize;
			glyph.size.Set(rect.w * _invSampleSize, rect.h * _invSampleSize);

			packingRects.push_back(rect);
		}

		packingNodes.resize(packingRects.size());
		stbrp_init_target(&packingContext, bitmapWidth, bitmapHeight, packingNodes.data(), packingNodes.size());
		stbrp_pack_rects(&packingContext, packingRects.data(), packingRects.size());

		for (stbrp_rect& rect: packingRects)
		{
			Glyph& glyph = glyphs[rect.id];
			glyph.x = rect.x * _normal.x;
			glyph.y = rect.y * _normal.y;

			int byteOffset = rect.x + (rect.y * bitmapWidth);
			stbtt_MakeCodepointBitmap(_info,
				bitmap.data() + byteOffset,
				rect.w,
				rect.h,
				bitmapWidth,
				scale,
				scale,
				rect.id);
		}

		_texture.LoadBitmap(bitmap.data(), bitmapWidth, bitmapHeight, 1, SamplerFilter::Nearest);
	}

	float Font::GetKerning(int c0, int c1)
	{
		return stbtt_GetCodepointKernAdvance(_info, c0, c1) * _invSampleSize;
	}

	float Font::GetLineHeight()
	{
		return _lineHeight;
	}

	int Font::GetSampleSize()
	{
		return _sampleSize;
	}

	Texture& Font::GetTexture()
	{
		return _texture;
	}

	const Texture& Font::GetTexture() const
	{
		return _texture;
	}

} // namespace Anggur
