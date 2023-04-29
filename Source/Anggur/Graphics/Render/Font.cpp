#include "Anggur/Math/Math.h"
#include "Anggur/Graphics/Render/Font.h"
#include "Anggur/Utility/UTF8.h"

#include <stb_image_write.h>
#include <stb_truetype.h>

namespace Anggur
{
	void Font::Read(const std::string& path)
	{
		data = File::Read(path);

		Initialize();
	}

	void Font::Initialize()
	{
		context = new FontContext;

		int result = stbtt_InitFont(context, data.data(), 0);

		if (!result)
			throw std::runtime_error("Failed to initilaze font [" + std::to_string(result) + "]");
	}

	void Font::SetSample(uint newContainerSize, uint newSampleSize, uint newSamplePadding, float newSampleRange)
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

		int advanceWidth, lsb;
		stbtt_GetCodepointHMetrics(context, 32, &advanceWidth, &lsb);

		float scale = stbtt_ScaleForPixelHeight(context, sampleSize);
		float sampleInverseScale = 1.0f / sampleSize;

		FontVMetrics vMetrics = GetVMetrics();

		spaceWidth = sampleInverseScale * scale * (advanceWidth - lsb);
		lineHeight = sampleInverseScale * (vMetrics.ascent - vMetrics.descent);
		lineGap = sampleInverseScale * vMetrics.lineGap;
	}

	std::string Font::GetName() 
	{
		int length = 0;
		const char* name = stbtt_GetFontNameDefault(context, &length);
		
		return std::string(name, length);
	}

	float Font::GetKerning(uint codePoint, uint nextCodePoint) 
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

	FontHMetrics Font::GetHMetrics(uint codepoint)
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

	bool Font::Generate(uint codepoint)
	{
		std::optional<FontBitmap> bitmap = GenerateBitmap(codepoint);

		if (!bitmap.has_value())
			return false;

		Pack(codepoint, bitmap.value());
		GenerateTexture();

		return true;
	}

	bool Font::GenerateRange(uint codepointFrom, uint length)
	{
		bool someFailed = false;

		for (uint i = 0; i < length; ++i)
		{
			uint codepoint = i + codepointFrom;
			
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

	std::optional<FontBitmap> Font::GenerateBitmap(uint codepoint)
	{
		FontBitmap bitmap;

		float scale = GetContextScale();

		bitmap.data = stbtt_GetCodepointSDF(
			context, scale, codepoint, samplePadding, 255, sampleRange, 
			&bitmap.width, &bitmap.height,
			&bitmap.x, &bitmap.y
		);

		if (bitmap.data == nullptr)
			return std::nullopt;

		return bitmap;
	}

	void Font::Pack(uint codepoint, const FontBitmap& bitmap)
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

		glyph.position.Set(sampleInverseScale * hMetrics.leftSideBearing, sampleInverseScale * (vMetrics.ascent + bitmap.y));
		glyph.size.Set(sampleInverseScale * bitmap.width, sampleInverseScale * bitmap.height);
		glyph.textureIndex = textureIndex;
		glyph.texturePosition = containerInverseScale * packer.GetPointer();
		glyph.textureSize.Set(containerInverseScale * bitmap.width, containerInverseScale * bitmap.height);

		glyphMap[codepoint] = glyph;

		packer.SetGlyph(bitmap.data, bitmap.width, bitmap.height);
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

	bool Font::GenerateASCII()
	{
		return GenerateRange(33, 94);
	}

	float Font::GetContextScale()
	{
		return stbtt_ScaleForPixelHeight(context, sampleSize);
	}
}

