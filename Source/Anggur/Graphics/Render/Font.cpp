#include "Anggur/Math/Math.h"
#include "Anggur/Graphics/Render/Font.h"

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
		spaceWidth = sampleInverseScale * scale * (advanceWidth - lsb);
	}

	float Font::GetKerning(uint codePoint, uint nextCodePoint) {
		float kerning = stbtt_GetCodepointKernAdvance(context, codePoint, nextCodePoint);
		float scale = stbtt_ScaleForPixelHeight(context, sampleSize);

		return kerning * scale / sampleSize;
	}

	std::string Font::GetName() 
	{
		int length = 0;
		const char* name = stbtt_GetFontNameDefault(context, &length);
		
		return std::string(name, length);
	}

	void Font::Generate(uint from, uint to)
	{
		float scale = stbtt_ScaleForPixelHeight(context, sampleSize);

		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(context, &ascent, &descent, &lineGap);

		ascent = roundf(ascent * scale);
		descent = roundf(descent * scale);

		int edgeValue = 255;
		float sampleInverseScale = 1.0f / sampleSize;
		float containerInverseScale = 1.0f / containerSize;

		for (uint i = 0; i < to; ++i)
		{
			int codePoint = i + from;
			int glyphWidth;
			int glyphHeight;
			int glyphX;
			int glyphY;

			int advanceWidth;
			int lsb;

			stbtt_GetCodepointHMetrics(context, codePoint, &advanceWidth, &lsb);

			uchar* buffer = stbtt_GetCodepointSDF(
				context, scale, codePoint, samplePadding, edgeValue, sampleRange, 
				&glyphWidth, &glyphHeight,
				&glyphX, &glyphY
			);

			if (buffer == nullptr)
				continue;

			if (!packer.IsFit(glyphWidth, glyphHeight))
			{
				++textureIndex;
				textures.push_back(new Texture2D(packer.image.GetBytes(), packer.image.GetWidth(), packer.image.GetHeight(), packer.image.GetChannels()));
				packer.Reset();
			}

			// Draw Bounding Box
			// for (uint x = 0; x < glyphWidth; ++x) 
			// {
			// 	buffer[x] = 255;
			// 	buffer[(glyphHeight - 1) * glyphWidth + x] = 255;
			// }
			//
			// for (uint y = 0; y < glyphHeight; ++y) 
			// {
			// 	buffer[y * glyphWidth] = 255;
			// 	buffer[y * glyphWidth + glyphWidth - 1] = 255;
			// }
			
			FontGlyph glyph;
			glyph.position.Set(sampleInverseScale * scale * lsb, sampleInverseScale * (glyphY + ascent));
			glyph.size.Set(sampleInverseScale * glyphWidth, sampleInverseScale * glyphHeight);
			glyph.textureIndex = textureIndex;
			glyph.texturePosition = containerInverseScale * packer.GetPointer();
			glyph.textureSize.Set(containerInverseScale * glyphWidth, containerInverseScale * glyphHeight);

			glyphMap[codePoint] = glyph;

			packer.SetGlyph(buffer, glyphWidth, glyphHeight);
		}

		textures.push_back(new Texture2D(packer.image.GetBytes(), packer.image.GetWidth(), packer.image.GetHeight(), packer.image.GetChannels()));
	}

	void Font::GenerateASCII()
	{
		Generate(33, 94);
	}
}

