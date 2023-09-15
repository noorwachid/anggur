#pragma once

#include "Anggur/Graphics/Font/FontGlyph.h"
#include "Anggur/Graphics/Font/FontPacker.h"
#include "Anggur/Graphics/Image.h"
#include "Anggur/Graphics/Texture.h"
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
			unsigned int newContainerSize, unsigned int newSampleSize, unsigned int newSamplePadding,
			float newSampleRange
		);

		float getKerning(unsigned int codePoint, unsigned int nextCodePoint);

		float getSpaceWidth() {
			return _spaceWidth;
		}

		float getLineHeight() {
			return _lineHeight;
		}

		float getLineGap() {
			return _lineGap;
		}

		FontVMetrics getVMetrics();

		FontHMetrics getHMetrics(unsigned int codepoint);

		const std::vector<Texture*>& getTextures() {
			return _textures;
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
		float _spaceWidth;
		float _lineHeight;
		float _lineGap;

		std::vector<Texture*> _textures;
		size_t _textureIndex = 0;

		FontContext* _context;
		std::vector<unsigned char> _data;

		std::unordered_map<unsigned int, FontGlyph> _glyphMap;

		FontPacker _packer;

		unsigned int _containerSize;
		unsigned int _sampleSize;
		unsigned int _samplePadding;
		float _sampleRange;
		float _sampleScale;

		friend class TextRenderer;
	};
}
