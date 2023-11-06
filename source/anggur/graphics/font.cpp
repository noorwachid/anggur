#include "anggur/graphics/font.h"
#include "anggur/math.h"

#include <stb_image_write.h>
#include <stb_truetype.h>

namespace Anggur {
	Font::Font() {
	}

	Font::Font(const std::vector<unsigned char>& data) {
		this->data = data;

		initialize();
	}

	Font::~Font() {
		for (Texture* texture : textures)
			delete texture;

		delete context;
	}

	void Font::setData(const std::vector<unsigned char>& data) {
		this->data = data;

		initialize();
	}

	void Font::initialize() {
		context = new FontContext;

		int result = stbtt_InitFont(context, data.data(), 0);

		if (!result)
			throw std::runtime_error("Failed to initilaze font [" + std::to_string(result) + "]");
	}

	void Font::setSample(
		unsigned int containerSize, unsigned int sampleSize, unsigned int samplePadding, float sampleRange
	) {
		if (containerSize < sampleSize) {
			throw std::runtime_error("Glyph atlas size must be greater than sampling size");
		}

		this->containerSize = containerSize;
		this->sampleSize = sampleSize;
		this->samplePadding = samplePadding;
		this->sampleRange = sampleRange;

		textureIndex = 0;

		for (Texture* texture : textures)
			delete texture;

		textures.clear();

		packer.reset();
		packer.setSize(containerSize);

		float sampleInverseScale = 1.0f / sampleSize;

		FontHMetrics hMetrics = getHMetrics(32);
		FontVMetrics vMetrics = getVMetrics();

		spaceWidth = sampleInverseScale * (hMetrics.advanceWidth - hMetrics.leftSideBearing);
		lineHeight = sampleInverseScale * (vMetrics.ascent - vMetrics.descent);
		lineGap = sampleInverseScale * vMetrics.lineGap;
	}

	std::string Font::getName() {
		int length = 0;
		const char* name = stbtt_GetFontNameDefault(context, &length);

		return std::string(name, length);
	}

	float Font::getKerning(unsigned int codePoint, unsigned int nextCodePoint) {
		float scale = getContextScale();
		float kerning = stbtt_GetCodepointKernAdvance(context, codePoint, nextCodePoint);

		return kerning * scale / sampleSize;
	}

	FontVMetrics Font::getVMetrics() {
		FontVMetrics metrics;

		float scale = getContextScale();

		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(context, &ascent, &descent, &lineGap);

		metrics.ascent = scale * ascent;
		metrics.descent = scale * descent;
		metrics.lineGap = scale * lineGap;

		return metrics;
	}

	FontHMetrics Font::getHMetrics(unsigned int codepoint) {
		FontHMetrics metrics;

		float scale = getContextScale();

		int advanceWidth;
		int leftSideBearing;

		stbtt_GetCodepointHMetrics(context, codepoint, &advanceWidth, &leftSideBearing);

		metrics.advanceWidth = scale * advanceWidth;
		metrics.leftSideBearing = scale * leftSideBearing;

		return metrics;
	}

	bool Font::generate(unsigned int codepoint) {
		std::optional<FontBitmap> bitmap = generateBitmap(codepoint);

		if (!bitmap.has_value())
			return false;

		pack(codepoint, bitmap.value());
		generateTexture();

		return true;
	}

	bool Font::generateRange(unsigned int codepointFrom, unsigned int length) {
		bool someFailed = false;

		for (unsigned int i = 0; i < length; ++i) {
			unsigned int codepoint = i + codepointFrom;

			std::optional<FontBitmap> bitmap = generateBitmap(codepoint);

			if (!bitmap.has_value()) {
				someFailed = true;
			} else {
				pack(codepoint, bitmap.value());
			}
		}

		generateTexture();

		return someFailed;
	}

	std::optional<FontBitmap> Font::generateBitmap(unsigned int codepoint) {
		FontBitmap bitmap;

		float scale = getContextScale();

		bitmap.data = stbtt_GetCodepointSDF(
			context, scale, codepoint, samplePadding, 128, sampleRange, &bitmap.width, &bitmap.height, &bitmap.x,
			&bitmap.y
		);

		if (bitmap.data == nullptr)
			return std::nullopt;

		return bitmap;
	}

	void Font::pack(unsigned int codepoint, const FontBitmap& bitmap) {
		FontVMetrics vMetrics = getVMetrics();
		FontHMetrics hMetrics = getHMetrics(codepoint);

		float sampleInverseScale = 1.0f / sampleSize;
		float containerInverseScale = 1.0f / containerSize;

		if (!packer.isFit(bitmap.width, bitmap.height)) {
			generateTexture();
			++textureIndex;
			packer.reset();
		}

		FontGlyph glyph;

		glyph.position.set(
			sampleInverseScale * hMetrics.leftSideBearing,
			sampleInverseScale * (vMetrics.ascent + bitmap.y + samplePadding)
		);
		glyph.size.set(sampleInverseScale * bitmap.width, sampleInverseScale * bitmap.height);
		glyph.textureIndex = textureIndex;
		glyph.texturePosition = containerInverseScale * packer.getPointer();
		glyph.textureSize.set(containerInverseScale * bitmap.width, containerInverseScale * bitmap.height);

		glyphMap[codepoint] = glyph;

		packer.setGlyph(bitmap.data, bitmap.width, bitmap.height);

		stbtt_FreeSDF(bitmap.data, nullptr);
	}

	void Font::generateTexture() {
		// Replace last texture
		if (textureIndex + 1 == textures.size()) {
			delete textures.back();
			textures.back() = new Texture(
				packer.image.getBytes(), packer.image.getWidth(), packer.image.getHeight(),
				packer.image.getChannels()
			);
		} else {
			textures.push_back(new Texture(
				packer.image.getBytes(), packer.image.getWidth(), packer.image.getHeight(),
				packer.image.getChannels()
			));
		}
	}

	bool Font::generateRC() {
		return generate(0xFFFD);
	}

	bool Font::generateEllipsis() {
		bool failed = false;

		if (!generate(0x2026))
			failed = true;

		if (!generate(0x22EE))
			failed = true;

		if (!generate(0x22EF))
			failed = true;

		if (!generate(0x22F0))
			failed = true;

		if (!generate(0x22F1))
			failed = true;

		return !failed;
	}

	bool Font::generateQuotationMark() {
		bool failed = false;

		if (!generate(0x2018))
			failed = true;

		if (!generate(0x2019))
			failed = true;

		if (!generate(0x201C))
			failed = true;

		if (!generate(0x201D))
			failed = true;

		return !failed;
	}

	bool Font::generateASCII() {
		return generateRange(33, 94);
	}

	float Font::getContextScale() {
		return stbtt_ScaleForPixelHeight(context, sampleSize);
	}
}
