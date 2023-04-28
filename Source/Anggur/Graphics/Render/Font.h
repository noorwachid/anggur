#pragma once

#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/Render/Image.h"
#include "Anggur/Graphics/Render/FontGlyph.h"
#include "Anggur/Graphics/Render/FontPacker.h"
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

	class Font
	{
	public:
		void Read(const std::string& path);

		void Initialize();

		void SetSample(uint newContainerSize, uint newSampleSize, uint newSamplePadding, float newSampleRange);

		float GetKerning(uint codePoint, uint nextCodePoint);

		float GetSpaceWidth() { return spaceWidth; }

		std::string GetName();

		void Generate(uint from = 'F', uint to = 1);

		void GenerateASCII();

	private:
		float spaceWidth;

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
