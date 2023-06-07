#include "Anggur/Graphics/Render/FontPacker.h"

namespace Anggur
{
	Vector2 FontPacker::GetPointer()
	{
		return Vector2(pointerX, pointerY);
	}

	bool FontPacker::IsFit(size_t width, size_t height)
	{
		if (pointerX + width > image.GetWidth()) 
		{
			pointerX = 0;
			pointerY += maxY;
			maxY = 0;
		}

		if (pointerY + height > image.GetHeight())
		{
			pointerX = 0;
			pointerY = 0;
			maxY = 0;

			return false;
		}

		return true;
	}

	void FontPacker::SetGlyph(unsigned char* bytes, size_t width, size_t height)
	{
		for (size_t x = 0; x < width; ++x) 
		{
			for (size_t y = 0; y < height; ++y) 
			{
				size_t glyphIndex = (y * width) + x;
				size_t containerIndex = ((y + pointerY) * image.GetWidth()) + x + pointerX;
				image.SetByte(containerIndex, bytes[glyphIndex]);
			}
		}

		pointerX += width;
		maxY = Math::Max(maxY, height);
	}

	void FontPacker::SetSize(unsigned int size)
	{
		image.SetSize(size, size);
		image.SetChannels(1);
		image.Resize();
	}

	void FontPacker::Reset()
	{
		maxY = 0;
		pointerX = 0;
		pointerY = 0;
		image.Reset();
	}
}

