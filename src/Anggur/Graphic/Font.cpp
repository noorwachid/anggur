#include <glad/glad.h>
#include <stb_image_write.h>
#include <stb_rect_pack.h>
#include <stb_truetype.h>
#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Utility/File.h>
#include <Anggur/Graphic/Font.h>

namespace Anggur {

	Font::Font() {
		initialize();
	}

	Font::~Font() {
		if (!info) {
			delete info;
			info = nullptr;
		}
	}

	void Font::initialize() {
		// ASCII
		for (int c = 33; c < 127; ++c)
			ranges.push_back(c);

		// // Latin1
		// for (int c = 161; c < 256; ++c) ranges.push_back(c);
		//
		// // LatinExtendedA
		// for (int c = 256; c < 384; ++c) ranges.push_back(c);
		//
		// // LatinExtendedB
		// for (int c = 384; c < 592; ++c) ranges.push_back(c);
	}

	void Font::load(const std::string& path, int sampleSize) {
		buffer = File::load(path);

		if (buffer.empty())
			std::exit(123);

		if (!info)
			info = new stbtt_fontinfo;

		this->sampleSize = sampleSize;
		this->invSampleSize = 1.f / sampleSize;

		int result = stbtt_InitFont(info, buffer.data(), 0);
		ANGGUR_ASSERT(result, "[Graphic.Font.load] Failed to load font");

		float scale = stbtt_ScaleForPixelHeight(info, sampleSize);
		int ascent, decent;
		stbtt_GetFontVMetrics(info, &ascent, &decent, 0);
		ascent *= scale;
		decent *= scale;
		lineHeight = (decent - ascent) * invSampleSize;

		// TODO: Check this boy, if it really the right way to do.
		int maxBufferSize;
		glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxBufferSize);
		int inSquareTermSize = Math::sqrt(maxBufferSize / 8);
		int bitmapWidth = inSquareTermSize;
		int bitmapHeight = inSquareTermSize;

		normal.set(1.0 / bitmapWidth, 1.0 / bitmapHeight);
		int bitmapSize = bitmapWidth * bitmapHeight;
		std::vector<uint8_t> bitmap(bitmapSize);

		int gX = 0;
		int gY = 0;

		stbrp_context packingContext;
		std::vector<stbrp_rect> packingRects;
		std::vector<stbrp_node> packingNodes;

		for (int c: ranges) {
			int ax, lsb;
			int x0, y0, x1, y1;
			stbtt_GetCodepointHMetrics(info, c, &ax, &lsb);
			stbtt_GetCodepointBitmapBox(info, c, scale, scale, &x0, &y0, &x1, &y1);

			stbrp_rect rect;
			rect.id = c;
			rect.x = 0;
			rect.y = 0;
			rect.w = x1 - x0;
			rect.h = y1 - y0;

			Glyph& glyph = glyphs[rect.id];
			glyph.w = rect.w * normal.x;
			glyph.h = rect.h * normal.y;
			glyph.ascent = (ascent + y0) * invSampleSize;
			glyph.size.set(rect.w * invSampleSize, rect.h * invSampleSize);

			packingRects.push_back(rect);
		}

		packingNodes.resize(packingRects.size());
		stbrp_init_target(&packingContext, bitmapWidth, bitmapHeight, packingNodes.data(), packingNodes.size());
		stbrp_pack_rects(&packingContext, packingRects.data(), packingRects.size());

		for (stbrp_rect& rect: packingRects) {
			Glyph& glyph = glyphs[rect.id];
			glyph.x = rect.x * normal.x;
			glyph.y = rect.y * normal.y;

			int byteOffset = rect.x + (rect.y * bitmapWidth);
			stbtt_MakeCodepointBitmap(info,
				bitmap.data() + byteOffset,
				rect.w,
				rect.h,
				bitmapWidth,
				scale,
				scale,
				rect.id);
		}

		texture.loadBitmap(bitmap.data(), bitmapWidth, bitmapHeight, 1, SamplerFilter::nearest);
	}

	float Font::getKerning(int c0, int c1) {
		return stbtt_GetCodepointKernAdvance(info, c0, c1) * invSampleSize;
	}

	float Font::getLineHeight() {
		return lineHeight;
	}

	int Font::getSampleSize() {
		return sampleSize;
	}

	Texture& Font::getTexture() {
		return texture;
	}

	const Texture& Font::getTexture() const {
		return texture;
	}
}
