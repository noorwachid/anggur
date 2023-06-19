#pragma once

#include "Anggur/Graphics/Render/Image.h"
#include "Anggur/Math/Math.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur {
	// NOTE: Using shelf rectangle packing algorithm
	// TODO: Use better algorithm
	class FontPacker {
	public:
		Vector2 getPointer();

		bool isFit(size_t width, size_t height);

		void setGlyph(unsigned char* bytes, size_t width, size_t height);

		void setSize(unsigned int size);

		void reset();

	public:
		unsigned int maxY = 0;
		unsigned int pointerX = 0;
		unsigned int pointerY = 0;

		Image image;
	};
}
