#pragma once

#include "anggur/graphics/font/fontglyph.h"
#include "anggur/graphics/font/fontpacker.h"
#include "anggur/graphics/image.h"
#include "anggur/graphics/texture.h"
#include <iostream>
#include <optional>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>

struct stbtt_fontinfo;

namespace Anggur {
	using FontContext = stbtt_fontinfo;

	struct FontVMetrics {
		float ascent;
		float descent;
		float lineGap;
	};

	struct FontHMetrics {
		float advanceWidth;
		float leftSideBearing;
	};

	struct FontBitmap {
		unsigned char* data;
		int x;
		int y;
		int width;
		int height;
	};

	class Font {
	public:
		Font();

		Font(const std::vector<unsigned char>& data);

		~Font();

		void setData(const std::vector<unsigned char>& data);

		void setSample(
			unsigned int containerSize, unsigned int sampleSize, unsigned int samplePadding, float sampleRange
		);

		float getKerning(unsigned int codePoint, unsigned int nextCodePoint);

		float getSpaceWidth() {
			return spaceWidth;
		}

		float getLineHeight() {
			return lineHeight;
		}

		float getLineGap() {
			return lineGap;
		}

		FontVMetrics getVMetrics();

		FontHMetrics getHMetrics(unsigned int codepoint);

		const std::vector<Texture*>& getTextures() {
			return textures;
		}

		std::string getName();

		bool generate(unsigned int codepoint);

		bool generateRange(unsigned int codepointFrom = 'F', unsigned int length = 1);

		bool generateRC();

		bool generateQuotationMark();

		bool generateEllipsis();

		bool generateASCII();

	private:
		void initialize();

		void pack(unsigned int codepoint, const FontBitmap& bitmap);

		std::optional<FontBitmap> generateBitmap(unsigned int codepoint);

		void generateTexture();

		float getContextScale();

	private:
		float spaceWidth;
		float lineHeight;
		float lineGap;

		std::vector<Texture*> textures;
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
