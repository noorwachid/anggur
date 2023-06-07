#pragma once

#include "Anggur/Graphics/Render/Image.h"
#include "Anggur/Math/Math.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur
{
	// NOTE: Using shelf rectangle packing algorithm
	// TODO: Use better algorithm
	class FontPacker
	{
	public:
		Vector2 GetPointer();

		bool IsFit(size_t width, size_t height);

		void SetGlyph(unsigned char* bytes, size_t width, size_t height);

		void SetSize(unsigned int size);

		void Reset();

	public:
		unsigned int maxY = 0;
		unsigned int pointerX = 0;
		unsigned int pointerY = 0;

		Image image;
	};
}
