#pragma once

#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/Render/Image.h"
#include "Anggur/Math/Math.h"
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

	class FontImagePacker
	{
	public:
		Vector2 GetPointer()
		{
			return Vector2(pointerX, pointerY);
		}

		bool IsFit(usize width, usize height)
		{
			if (pointerX + width > image.GetWidth()) 
			{
				pointerX = 0;
				pointerY += maxY;
			}

			if (pointerY + height > image.GetHeight())
			{
				pointerX = 0;
				pointerY = 0;

				return false;
			}

			return true;
		}

		void SetGlyph(uchar* bytes, usize width, usize height)
		{
			for (usize x = 0; x < width; ++x) 
			{
				for (usize y = 0; y < height; ++y) 
				{
					usize glyphIndex = (y * width) + x;
					usize containerIndex = ((y + pointerY) * image.GetWidth()) + x + pointerX;
					image.SetByte(containerIndex, bytes[glyphIndex]);
				}
			}

			pointerX += width;
			maxY = Math::Max(maxY, height);
		}

		void SetSize(uint size)
		{
			image.SetSize(size, size);
			image.SetChannels(1);
			image.Resize();
		}

		void Reset()
		{
			maxY = 0;
			pointerX = 0;
			pointerY = 0;
			image.Reset();
		}

	public:
		uint maxY = 0;
		uint pointerX = 0;
		uint pointerY = 0;

		Image image;

	};

	struct FontGlyph
	{
		Vector2 position;
		Vector2 size;

		usize textureIndex = 0;

		Vector2 texturePosition;
		Vector2 textureSize;
	};

	class Font
	{
	public:
		std::vector<Texture2D*> textures;
		usize textureIndex = 0;

		FontContext* context;
		std::vector<uchar> data;

		std::unordered_map<uint, FontGlyph> glyphMap;

		FontImagePacker imagePacker;

		uint containerSize;
		uint sampleSize;
		uint samplePadding;
		float sampleRange;

		float sampleScale;

		void Read(const std::string& path);

		void Initialize();

		void SetSample(uint newContainerSize, uint newSampleSize, uint newSamplePadding, float newSampleRange);

		float GetKerning(uint codePoint, uint nextCodePoint);

		std::string GetName();

		void Generate(uint from = 'F', uint to = 1);

		void GenerateASCII();
	};
}
