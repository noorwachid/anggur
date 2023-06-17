#pragma once

#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/Render/Image.h"
#include "Anggur/Graphics/Render/FontGlyph.h"
#include "Anggur/Graphics/Render/FontPacker.h"
#include <iostream>
#include <optional>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>

struct stbtt_fontinfo;

namespace Anggur
{
	using FontContext = stbtt_fontinfo;

	struct FontVMetrics
	{
		float ascent;
		float descent;
		float lineGap;
	};

	struct FontHMetrics
	{
		float advanceWidth;
		float leftSideBearing;
	};

	struct FontBitmap
	{
		unsigned char* data;
		int x;
		int y;
		int width;
		int height;
	};

	class Font
	{
	public:
		Font();

		Font(const std::vector<unsigned char>& data);

		~Font();

		void SetData(const std::vector<unsigned char>& data);

		void SetSample(unsigned int newContainerSize, unsigned int newSampleSize, unsigned int newSamplePadding, float newSampleRange);

		float GetKerning(unsigned int codePoint, unsigned int nextCodePoint);

		float GetSpaceWidth() { return spaceWidth; }

		float GetLineHeight() { return lineHeight; }

		float GetLineGap() { return lineGap; }

		FontVMetrics GetVMetrics();

		FontHMetrics GetHMetrics(unsigned int codepoint);

		const std::vector<Texture2D*>& GetTextures() { return textures; }

		std::string GetName();

		bool Generate(unsigned int codepoint);

		bool GenerateRange(unsigned int codepointFrom = 'F', unsigned int length = 1);

		bool GenerateRC();

		bool GenerateQuotationMark();

		bool GenerateEllipsis();

		bool GenerateASCII();

	private:
		void Initialize();

		void Pack(unsigned int codepoint, const FontBitmap& bitmap);

		std::optional<FontBitmap> GenerateBitmap(unsigned int codepoint);

		void GenerateTexture();

		float GetContextScale();

	private:
		float spaceWidth;
		float lineHeight;
		float lineGap;

		std::vector<Texture2D*> textures;
		size_t textureIndex = 0;

		FontContext* context;
		std::vector<unsigned char> data;

		std::unordered_map<unsigned int, FontGlyph> glyphMap;

		FontPacker packer;

		unsigned int containerSize;
		unsigned int sampleSize;
		unsigned int samplePadding;
		float sampleRange;

		float sampleScale;

		friend class TextRenderer;
	};
}
