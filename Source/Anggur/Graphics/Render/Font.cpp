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

	void Font::SetSample(const std::string& newPath, uint newGlyphSamplingSize, uint newGlyphAtlasSize)
	{
		Read(newPath);
		glyphSamplingSize = newGlyphSamplingSize;
		glyphAtlasSize = newGlyphAtlasSize;

		if (glyphAtlasSize < glyphSamplingSize)
		{
			throw std::runtime_error("Glyph atlas size must be greater than sampling size");
		}
	}

	float Font::GetKerning(uint codePoint, uint nextCodePoint) {
		float kerning = stbtt_GetCodepointKernAdvance(context, codePoint, nextCodePoint);
		float scale = stbtt_ScaleForPixelHeight(context, glyphSamplingSize);

		return kerning * scale * glyphNormal;
	}

	std::string Font::GetName() 
	{
		int length = 0;
		const char* name = stbtt_GetFontNameDefault(context, &length);
		
		return std::string(name, length);
	}

	void Font::Generate(uint newCodePoint, uint next)
	{
		if (glyphBuffers.empty() || glyphBuffers.back().occupied)
			glyphBuffers.push_back(GlyphBuffer(glyphAtlasSize));

		float scale = stbtt_ScaleForPixelHeight(context, glyphSamplingSize);

		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(context, &ascent, &descent, &lineGap);

		ascent = roundf(ascent * scale);
		descent = roundf(descent * scale);

		float pixelDistanceScale = 32.0; // trades off precision w/ ability to handle *smaller* sizes
		int edgeValue = 206;
		int padding = 6; // not used in shade
		float normal = 1.0f / glyphSamplingSize;
		float textureNormal = 1.0f / glyphAtlasSize;

		glyphSamplingPaddingSize = padding;
		glyphNormal = normal;
		glyphPadding = padding * normal;

		for (uint i = 0; i < next; ++i)
		{
			int codePoint = i + newCodePoint;
			int glyphWidth;
			int glyphHeight;
			int glyphX;
			int glyphY;

			uchar* buffer = stbtt_GetCodepointSDF(
				context, scale, codePoint, padding, edgeValue, pixelDistanceScale, &glyphWidth, &glyphHeight,
				&glyphX, &glyphY
			);

			if (glyphMaxHeight < glyphHeight)
				glyphMaxHeight = glyphHeight;

			if (glyphBuffers.back().pointerX + glyphWidth > glyphBuffers.back().image.GetWidth())
			{
				glyphBuffers.back().pointerX = 0;
				glyphBuffers.back().pointerY += glyphMaxHeight;

				glyphMaxHeight = 0;
			}

			if (glyphBuffers.back().pointerY + glyphHeight > glyphBuffers.back().image.GetHeight())
			{
				glyphBuffers.back().occupied = true;

				if (glyphBuffers.back().dirty)
				{
					Image& image = glyphBuffers.back().image;
					glyphBuffers.back().texture = new Texture2D(image.GetBytes(), image.GetWidth(), image.GetHeight(), image.GetChannels());
					glyphBuffers.back().dirty = false;
				}

				glyphBuffers.push_back(GlyphBuffer(glyphAtlasSize));
			}

			Glyph glyph;

			glyph.size.Set(glyphWidth * normal, glyphHeight * normal);
			glyph.ascent = (ascent + glyphY) * normal;
			glyph.descent = -descent * normal;
			glyph.bufferIndex = glyphBuffers.size() - 1;

			glyph.texturePosition.Set((glyphBuffers.back().pointerX) * textureNormal, glyphBuffers.back().pointerY * textureNormal);
			glyph.textureSize.Set(glyphWidth * textureNormal, glyphHeight * textureNormal);

			glyphMap[codePoint] = glyph;

			for (int x = 0; x < glyphWidth; ++x)
				for (int y = 0; y < glyphHeight; ++y)
					glyphBuffers.back().image.SetByte(
						x + glyphBuffers.back().pointerX, y + glyphBuffers.back().pointerY,
						buffer[y * glyphWidth + x]
					);

			glyphBuffers.back().pointerX += glyphWidth;
		}

		Image& image = glyphBuffers.back().image;
		glyphBuffers.back().texture = new Texture2D(image.GetBytes(), image.GetWidth(), image.GetHeight(), image.GetChannels());
	}

	void Font::GenerateASCII()
	{
		Generate(33, 94);
	}
}

