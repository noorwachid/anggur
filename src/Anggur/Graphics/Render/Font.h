#pragma once

#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/Render/Image.h"
#include "Anggur/Graphics/Render/FontGlyph.h"
#include "Anggur/Graphics/Render/FontPacker.h"
#include "Anggur/IO/File.h"
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
		uchar* data;
		int x;
		int y;
		int width;
		int height;
	};

	class Font
	{
	public:
		void Read(const std::string& path);

		void Initialize();

		void SetSample(uint newContainerSize, uint newSampleSize, uint newSamplePadding, float newSampleRange);

		float GetKerning(uint codePoint, uint nextCodePoint);

		float GetSpaceWidth() { return spaceWidth; }

		float GetLineHeight() { return lineHeight; }

		float GetLineGap() { return lineGap; }

		FontVMetrics GetVMetrics();

		FontHMetrics GetHMetrics(uint codepoint);

		const std::vector<Texture2D*>& GetTextures() { return textures; }

		std::string GetName();

		bool Generate(uint codepoint);

		bool GenerateRange(uint codepointFrom = 'F', uint length = 1);

		bool GenerateRC();

		bool GenerateQuotationMark();

		bool GenerateEllipsis();

		bool GenerateASCII();

	private:
		void Pack(uint codepoint, const FontBitmap& bitmap);

		std::optional<FontBitmap> GenerateBitmap(uint codepoint);

		void GenerateTexture();

		float GetContextScale();

	private:
		float spaceWidth;
		float lineHeight;
		float lineGap;

		std::vector<Texture2D*> textures;
		usize textureIndex = 0;

		FontContext* context;
		std::vector<uchar> data;

		std::unordered_map<uint, FontGlyph> glyphMap;

		FontPacker packer;

		uint containerSize;
		uint sampleSize;
		uint samplePadding;
		float sampleRange;

		float sampleScale;

		friend class TextRenderer;
	};
}
