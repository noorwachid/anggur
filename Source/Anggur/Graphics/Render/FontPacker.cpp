#include "Anggur/Graphics/Render/FontPacker.h"

namespace Anggur
{
	Vector2 FontPacker::GetPointer()
	{
		return Vector2(pointerX, pointerY);
	}

	bool FontPacker::IsFit(usize width, usize height)
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

	void FontPacker::SetGlyph(uchar* bytes, usize width, usize height)
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

	void FontPacker::SetSize(uint size)
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

