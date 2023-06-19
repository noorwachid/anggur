#include "Anggur/Graphics/Render/FontPacker.h"

namespace Anggur {
	Vector2 FontPacker::getPointer() {
		return Vector2(pointerX, pointerY);
	}

	bool FontPacker::isFit(size_t width, size_t height) {
		if (pointerX + width > image.getWidth()) {
			pointerX = 0;
			pointerY += maxY;
			maxY = 0;
		}

		if (pointerY + height > image.getHeight()) {
			pointerX = 0;
			pointerY = 0;
			maxY = 0;

			return false;
		}

		return true;
	}

	void FontPacker::setGlyph(unsigned char* bytes, size_t width, size_t height) {
		for (size_t x = 0; x < width; ++x) {
			for (size_t y = 0; y < height; ++y) {
				size_t glyphIndex = (y * width) + x;
				size_t containerIndex = ((y + pointerY) * image.getWidth()) + x + pointerX;
				image.setByte(containerIndex, bytes[glyphIndex]);
			}
		}

		pointerX += width;
		maxY = Math::max(maxY, height);
	}

	void FontPacker::setSize(unsigned int size) {
		image.setSize(size, size);
		image.setChannels(1);
		image.resize();
	}

	void FontPacker::reset() {
		maxY = 0;
		pointerX = 0;
		pointerY = 0;
		image.reset();
	}
}
