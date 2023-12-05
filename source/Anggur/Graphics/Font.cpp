#include "Anggur/Graphics/Font.h"
#include "Anggur/Math.h"

#include <stb_image_write.h>
#include <stb_truetype.h>

namespace Anggur
{
	Font::Font()
	{
	}

	Font::Font(const std::vector<unsigned char>& data)
	{
		_data = data;

		Initialize();
	}

	Font::~Font()
	{
		for (Texture* texture : _textures)
			delete texture;

		delete _context;
	}

	void Font::SetData(const std::vector<unsigned char>& data)
	{
		_data = data;

		Initialize();
	}

	void Font::Initialize()
	{
		_context = new FontContext;

		int result = stbtt_InitFont(_context, _data.data(), 0);

		if (!result)
			throw std::runtime_error("Failed to initilaze font [" + std::to_string(result) + "]");
	}

	void Font::SetSample(
		unsigned int containerSize, unsigned int sampleSize, unsigned int samplePadding, float sampleRange
	)
	{
		if (containerSize < sampleSize)
		{
			throw std::runtime_error("Glyph atlas size must be greater than sampling size");
		}

		_containerSize = containerSize;
		_sampleSize = sampleSize;
		_samplePadding = samplePadding;
		_sampleRange = sampleRange;

		_textureIndex = 0;

		for (Texture* texture : _textures)
			delete texture;

		_textures.clear();

		_packer.Reset();
		_packer.SetSize(containerSize);

		float sampleInverseScale = 1.0f / sampleSize;

		FontHMetrics hMetrics = GetHMetrics(32);
		FontVMetrics vMetrics = GetVMetrics();

		_spaceWidth = sampleInverseScale * (hMetrics.advanceWidth - hMetrics.leftSideBearing);
		_lineHeight = sampleInverseScale * (vMetrics.ascent - vMetrics.descent);
		_lineGap = sampleInverseScale * vMetrics.lineGap;
	}

	std::string Font::GetName()
	{
		int length = 0;
		const char* name = stbtt_GetFontNameDefault(_context, &length);

		return std::string(name, length);
	}

	float Font::GetKerning(unsigned int codePoint, unsigned int nextCodePoint)
	{
		float scale = GetContextScale();
		float kerning = stbtt_GetCodepointKernAdvance(_context, codePoint, nextCodePoint);

		return kerning * scale / _sampleSize;
	}

	FontVMetrics Font::GetVMetrics()
	{
		FontVMetrics metrics;

		float scale = GetContextScale();

		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(_context, &ascent, &descent, &lineGap);

		metrics.ascent = scale * ascent;
		metrics.descent = scale * descent;
		metrics.lineGap = scale * lineGap;

		return metrics;
	}

	FontHMetrics Font::GetHMetrics(unsigned int codepoint)
	{
		FontHMetrics metrics;

		float scale = GetContextScale();

		int advanceWidth;
		int leftSideBearing;

		stbtt_GetCodepointHMetrics(_context, codepoint, &advanceWidth, &leftSideBearing);

		metrics.advanceWidth = scale * advanceWidth;
		metrics.leftSideBearing = scale * leftSideBearing;

		return metrics;
	}

	bool Font::Generate(unsigned int codepoint)
	{
		std::optional<FontBitmap> bitmap = GenerateBitmap(codepoint);

		if (!bitmap.has_value())
			return false;

		Pack(codepoint, bitmap.value());
		GenerateTexture();

		return true;
	}

	bool Font::GenerateRange(unsigned int codepointFrom, unsigned int length)
	{
		bool someFailed = false;

		for (unsigned int i = 0; i < length; ++i)
		{
			unsigned int codepoint = i + codepointFrom;

			std::optional<FontBitmap> bitmap = GenerateBitmap(codepoint);

			if (!bitmap.has_value())
			{
				someFailed = true;
			}
			else
			{
				Pack(codepoint, bitmap.value());
			}
		}

		GenerateTexture();

		return someFailed;
	}

	std::optional<FontBitmap> Font::GenerateBitmap(unsigned int codepoint)
	{
		FontBitmap bitmap;

		float scale = GetContextScale();

		bitmap.data = stbtt_GetCodepointSDF(
			_context, scale, codepoint, _samplePadding, 128, _sampleRange, &bitmap.width, &bitmap.height, &bitmap.x,
			&bitmap.y
		);

		if (bitmap.data == nullptr)
			return std::nullopt;

		return bitmap;
	}

	void Font::Pack(unsigned int codepoint, const FontBitmap& bitmap)
	{
		FontVMetrics vMetrics = GetVMetrics();
		FontHMetrics hMetrics = GetHMetrics(codepoint);

		float sampleInverseScale = 1.0f / _sampleSize;
		float containerInverseScale = 1.0f / _containerSize;

		if (!_packer.IsFit(bitmap.width, bitmap.height))
		{
			GenerateTexture();
			++_textureIndex;
			_packer.Reset();
		}

		FontGlyph glyph;

		glyph.position.Set(
			sampleInverseScale * hMetrics.leftSideBearing,
			sampleInverseScale * (vMetrics.ascent + bitmap.y + _samplePadding)
		);
		glyph.size.Set(sampleInverseScale * bitmap.width, sampleInverseScale * bitmap.height);
		glyph.textureIndex = _textureIndex;
		glyph.texturePosition = containerInverseScale * _packer.GetPointer();
		glyph.textureSize.Set(containerInverseScale * bitmap.width, containerInverseScale * bitmap.height);

		_glyphMap[codepoint] = glyph;

		_packer.SetGlyph(bitmap.data, bitmap.width, bitmap.height);

		stbtt_FreeSDF(bitmap.data, nullptr);
	}

	void Font::GenerateTexture()
	{
		// Replace last texture
		if (_textureIndex + 1 == _textures.size())
		{
			delete _textures.back();
			_textures.back() = new Texture(
				_packer.image.GetBytes(), _packer.image.GetWidth(), _packer.image.GetHeight(), _packer.image.GetChannels()
			);
		}
		else
		{
			_textures.push_back(new Texture(
				_packer.image.GetBytes(), _packer.image.GetWidth(), _packer.image.GetHeight(), _packer.image.GetChannels()
			));
		}
	}

	bool Font::GenerateRC()
	{
		return Generate(0xFFFD);
	}

	bool Font::GenerateEllipsis()
	{
		bool failed = false;

		if (!Generate(0x2026))
			failed = true;

		if (!Generate(0x22EE))
			failed = true;

		if (!Generate(0x22EF))
			failed = true;

		if (!Generate(0x22F0))
			failed = true;

		if (!Generate(0x22F1))
			failed = true;

		return !failed;
	}

	bool Font::GenerateQuotationMark()
	{
		bool failed = false;

		if (!Generate(0x2018))
			failed = true;

		if (!Generate(0x2019))
			failed = true;

		if (!Generate(0x201C))
			failed = true;

		if (!Generate(0x201D))
			failed = true;

		return !failed;
	}

	bool Font::GenerateASCII()
	{
		return GenerateRange(33, 94);
	}

	float Font::GetContextScale()
	{
		return stbtt_ScaleForPixelHeight(_context, _sampleSize);
	}
}
