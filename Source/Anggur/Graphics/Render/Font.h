#pragma once

#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/Render/Image.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/IO/File.h"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <stb_image_write.h>
#include <stb_truetype.h>

struct stbtt_fontinfo;

namespace Anggur
{
	using FontContext = stbtt_fontinfo;

	struct Glyph
	{
		Vector2 size;

		float ascent;
		float descent;

		size_t bufferIndex = 0;

		Vector2 texturePosition;
		Vector2 textureSize;
	};

	struct GlyphBuffer
	{
		uint32_t pointerX = 0;
		uint32_t pointerY = 0;

		bool occupied = false;
		bool dirty = true;

		Image image;
		Texture2D* texture;

		GlyphBuffer(uint32_t size)
		{
			image.SetSize(size, size);
			image.SetChannels(1);
			image.Resize();
		}
	};

	struct Font
	{
		FontContext* context;
		std::vector<uint8_t> data;

		float glyphNormal;
		float glyphPadding;

		uint glyphSamplingPaddingSize;
		uint32_t glyphSamplingSize;
		uint32_t glyphAtlasSize;
		uint32_t glyphMaxHeight = 0;

		std::vector<GlyphBuffer> glyphBuffers;
		std::unordered_map<uint32_t, Glyph> glyphMap;

		void Read(const std::string& path)
		{
			FILE* fontFile = fopen(path.c_str(), "rb");
			fseek(fontFile, 0, SEEK_END);
			size_t byteSize = ftell(fontFile);
			fseek(fontFile, 0, SEEK_SET);

			data.resize(byteSize);

			fread(data.data(), byteSize, 1, fontFile);
			fclose(fontFile);

			Initialize();
		}

		void Initialize()
		{
			context = new FontContext;

			int result = stbtt_InitFont(context, data.data(), 0);

			if (!result)
				throw std::runtime_error("Failed to initilaze font [" + std::to_string(result) + "]");
		}

		void SetSample(const std::string& newPath, uint32_t newGlyphSamplingSize, uint32_t newGlyphAtlasSize)
		{
			Read(newPath);
			glyphSamplingSize = newGlyphSamplingSize;
			glyphAtlasSize = newGlyphAtlasSize;

			if (glyphAtlasSize < glyphSamplingSize)
			{
				throw std::runtime_error("Glyph atlas size must be greater than sampling size");
			}
		}

		float GetKerning(uint codePoint, uint nextCodePoint) {
			float kerning = stbtt_GetCodepointKernAdvance(context, codePoint, nextCodePoint);
			float scale = stbtt_ScaleForPixelHeight(context, glyphSamplingSize);

			return kerning * scale * glyphNormal;
		}

		void Generate(uint32_t newCodePoint = 'F', uint32_t next = 1)
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

			for (uint32_t i = 0; i < next; ++i)
			{
				int codePoint = i + newCodePoint;
				int glyphWidth;
				int glyphHeight;
				int glyphX;
				int glyphY;

				uint8_t* buffer = stbtt_GetCodepointSDF(
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

		void GenerateASCII()
		{
			Generate(33, 94);
		}
	};
}