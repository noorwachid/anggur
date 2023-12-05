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

		void SetSample(
			unsigned int containerSize, unsigned int sampleSize, unsigned int samplePadding, float sampleRange
		);

		float GetKerning(unsigned int codePoint, unsigned int nextCodePoint);

		float GetSpaceWidth()
		{
			return _spaceWidth;
		}

		float GetLineHeight()
		{
			return _lineHeight;
		}

		float GetLineGap()
		{
			return _lineGap;
		}

		FontVMetrics GetVMetrics();

		FontHMetrics GetHMetrics(unsigned int codepoint);

		const std::vector<Texture*>& GetTextures()
		{
			return _textures;
		}

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
