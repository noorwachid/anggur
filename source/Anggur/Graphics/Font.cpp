#include "Anggur/Graphics/Font.h"
#include "Anggur/Math.h"
#include "Anggur/Text.h"

#include <stb_image_write.h>
#include <stb_truetype.h>

namespace Anggur {
	Font::Font() {
	}

	Font::Font(const std::vector<unsigned char>& data) {
		_data = data;

		initialize();
	}

	Font::~Font() {
		for (Texture* texture : _textures)
			delete texture;

		delete _context;
	}

	void Font::setData(const std::vector<unsigned char>& data) {
		_data = data;

		initialize();
	}

	void Font::initialize() {
		_context = new FontContext;

		int result = stbtt_InitFont(_context, _data.data(), 0);

		if (!result)
			throw std::runtime_error("Failed to initilaze font [" + std::to_string(result) + "]");
	}

	void Font::setSample(
		unsigned int newContainerSize, unsigned int newSampleSize, unsigned int newSamplePadding, float newSampleRange
	) {
		if (newContainerSize < newSampleSize) {
			throw std::runtime_error("Glyph atlas size must be greater than sampling size");
		}

		_containerSize = newContainerSize;
		_sampleSize = newSampleSize;
		_samplePadding = newSamplePadding;
		_sampleRange = newSampleRange;

		_textureIndex = 0;

		for (Texture* texture : _textures)
			delete texture;

		_textures.clear();

		_packer.reset();
		_packer.setSize(_containerSize);

		float sampleInverseScale = 1.0f / _sampleSize;

		FontHMetrics hMetrics = getHMetrics(32);
		FontVMetrics vMetrics = getVMetrics();

		_spaceWidth = sampleInverseScale * (hMetrics.advanceWidth - hMetrics.leftSideBearing);
		_lineHeight = sampleInverseScale * (vMetrics.ascent - vMetrics.descent);
		_lineGap = sampleInverseScale * vMetrics.lineGap;
	}

	std::string Font::getName() {
		int length = 0;
		const char* name = stbtt_GetFontNameDefault(_context, &length);

		return std::string(name, length);
	}

	float Font::getKerning(unsigned int codePoint, unsigned int nextCodePoint) {
		float scale = getContextScale();
		float kerning = stbtt_GetCodepointKernAdvance(_context, codePoint, nextCodePoint);

		return kerning * scale / _sampleSize;
	}

	FontVMetrics Font::getVMetrics() {
		FontVMetrics metrics;

		float scale = getContextScale();

		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(_context, &ascent, &descent, &lineGap);

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

		stbtt_GetCodepointHMetrics(_context, codepoint, &advanceWidth, &leftSideBearing);

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
			_context, scale, codepoint, _samplePadding, 128, _sampleRange, &bitmap.width, &bitmap.height, &bitmap.x,
			&bitmap.y
		);

		if (bitmap.data == nullptr)
			return std::nullopt;

		return bitmap;
	}

	void Font::pack(unsigned int codepoint, const FontBitmap& bitmap) {
		FontVMetrics vMetrics = getVMetrics();
		FontHMetrics hMetrics = getHMetrics(codepoint);

		float sampleInverseScale = 1.0f / _sampleSize;
		float containerInverseScale = 1.0f / _containerSize;

		if (!_packer.isFit(bitmap.width, bitmap.height)) {
			generateTexture();
			++_textureIndex;
			_packer.reset();
		}

		FontGlyph glyph;

		glyph.position.set(
			sampleInverseScale * hMetrics.leftSideBearing,
			sampleInverseScale * (vMetrics.ascent + bitmap.y + _samplePadding)
		);
		glyph.size.set(sampleInverseScale * bitmap.width, sampleInverseScale * bitmap.height);
		glyph.textureIndex = _textureIndex;
		glyph.texturePosition = containerInverseScale * _packer.getPointer();
		glyph.textureSize.set(containerInverseScale * bitmap.width, containerInverseScale * bitmap.height);

		_glyphMap[codepoint] = glyph;

		_packer.setGlyph(bitmap.data, bitmap.width, bitmap.height);

		stbtt_FreeSDF(bitmap.data, nullptr);
	}

	void Font::generateTexture() {
		// Replace last texture
		if (_textureIndex + 1 == _textures.size()) {
			delete _textures.back();
			_textures.back() = new Texture(
				_packer.image.getBytes(), _packer.image.getWidth(), _packer.image.getHeight(),
				_packer.image.getChannels()
			);
		} else {
			_textures.push_back(new Texture(
				_packer.image.getBytes(), _packer.image.getWidth(), _packer.image.getHeight(),
				_packer.image.getChannels()
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
		return stbtt_ScaleForPixelHeight(_context, _sampleSize);
	}
}
