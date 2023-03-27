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
		uint pointerX = 0;
		uint pointerY = 0;

		bool occupied = false;
		bool dirty = true;

		Image image;
		Texture2D* texture;

		GlyphBuffer(uint size)
		{
			image.SetSize(size, size);
			image.SetChannels(1);
			image.Resize();
		}
	};

	class Font
	{
	public:
		FontContext* context;
		std::vector<uchar> data;

		float glyphNormal;
		float glyphPadding;

		uint glyphSamplingPaddingSize;
		uint glyphSamplingSize;
		uint glyphAtlasSize;
		uint glyphMaxHeight = 0;

		std::vector<GlyphBuffer> glyphBuffers;
		std::unordered_map<uint, Glyph> glyphMap;

		void Read(const std::string& path);

		void Initialize();

		void SetSample(const std::string& newPath, uint newGlyphSamplingSize, uint newGlyphAtlasSize);

		float GetKerning(uint codePoint, uint nextCodePoint);

		std::string GetName();

		void Generate(uint newCodePoint = 'F', uint next = 1);

		void GenerateASCII();
	};
}
