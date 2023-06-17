#include "Anggur/Core/Text.h"
#include "Anggur/Math/Math.h"
#include "Anggur/Graphics/Render/Font.h"

#include <stb_image_write.h>
#include <stb_truetype.h>

namespace Anggur
{
	Font::Font()
	{
	}

	Font::Font(const std::vector<unsigned char>& data)
	{
		this->data = data;

		Initialize();
	}

	Font::~Font()
	{
		for (Texture2D* texture : textures)
			delete texture;

		delete context;
	}

	void Font::SetData(const std::vector<unsigned char>& data)
	{
		this->data = data;

		Initialize();
	}

	void Font::Initialize()
	{
		context = new FontContext;

		int result = stbtt_InitFont(context, data.data(), 0);

		if (!result)
			throw std::runtime_error("Failed to initilaze font [" + std::to_string(result) + "]");
	}

	void Font::SetSample(unsigned int newContainerSize, unsigned int newSampleSize, unsigned int newSamplePadding, float newSampleRange)
	{
		if (newContainerSize < newSampleSize)
		{
			throw std::runtime_error("Glyph atlas size must be greater than sampling size");
		}

		containerSize = newContainerSize;
		sampleSize = newSampleSize;
		samplePadding = newSamplePadding;
		sampleRange = newSampleRange;

		textureIndex = 0;
		
		for (Texture2D* texture : textures)
			delete texture;
		
		textures.clear();

		packer.Reset();
		packer.SetSize(containerSize);

		float sampleInverseScale = 1.0f / sampleSize;

		FontHMetrics hMetrics = GetHMetrics(32);
		FontVMetrics vMetrics = GetVMetrics();

		spaceWidth = sampleInverseScale * (hMetrics.advanceWidth - hMetrics.leftSideBearing);
		lineHeight = sampleInverseScale * (vMetrics.ascent - vMetrics.descent);
		lineGap = sampleInverseScale * vMetrics.lineGap;
	}

	std::string Font::GetName() 
	{
		int length = 0;
		const char* name = stbtt_GetFontNameDefault(context, &length);
		
		return std::string(name, length);
	}

	float Font::GetKerning(unsigned int codePoint, unsigned int nextCodePoint) 
	{
		float scale = GetContextScale();
		float kerning = stbtt_GetCodepointKernAdvance(context, codePoint, nextCodePoint);

		return kerning * scale / sampleSize;
	}

	FontVMetrics Font::GetVMetrics()
	{
		FontVMetrics metrics;

		float scale = GetContextScale();

		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(context, &ascent, &descent, &lineGap);

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

		stbtt_GetCodepointHMetrics(context, codepoint, &advanceWidth, &leftSideBearing);
		
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
			context, scale, codepoint, samplePadding, 128, sampleRange, 
			&bitmap.width, &bitmap.height,
			&bitmap.x, &bitmap.y
		);

		if (bitmap.data == nullptr)
			return std::nullopt;

		return bitmap;
	}

	void Font::Pack(unsigned int codepoint, const FontBitmap& bitmap)
	{
		FontVMetrics vMetrics = GetVMetrics();
		FontHMetrics hMetrics = GetHMetrics(codepoint);

		float sampleInverseScale = 1.0f / sampleSize;
		float containerInverseScale = 1.0f / containerSize;

		if (!packer.IsFit(bitmap.width, bitmap.height))
		{
			GenerateTexture();
			++textureIndex;
			packer.Reset();
		}
		
		FontGlyph glyph;

		glyph.position.Set(sampleInverseScale * hMetrics.leftSideBearing, sampleInverseScale * (vMetrics.ascent + bitmap.y + samplePadding));
		glyph.size.Set(sampleInverseScale * bitmap.width, sampleInverseScale * bitmap.height);
		glyph.textureIndex = textureIndex;
		glyph.texturePosition = containerInverseScale * packer.GetPointer();
		glyph.textureSize.Set(containerInverseScale * bitmap.width, containerInverseScale * bitmap.height);

		glyphMap[codepoint] = glyph;

		packer.SetGlyph(bitmap.data, bitmap.width, bitmap.height);

		stbtt_FreeSDF(bitmap.data, nullptr);
	}

	void Font::GenerateTexture()
	{
		// Replace last texture
		if (textureIndex + 1 == textures.size()) 
		{
			delete textures.back();
			textures.back() = new Texture2D(packer.image.GetBytes(), packer.image.GetWidth(), packer.image.GetHeight(), packer.image.GetChannels());
		}
		else
		{
			textures.push_back(new Texture2D(packer.image.GetBytes(), packer.image.GetWidth(), packer.image.GetHeight(), packer.image.GetChannels()));
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
		return stbtt_ScaleForPixelHeight(context, sampleSize);
	}
}

