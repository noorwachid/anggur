#pragma once

#include "Anggur/Common/Integral.h"
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

		bool IsFit(usize width, usize height);

		void SetGlyph(uchar* bytes, usize width, usize height);

		void SetSize(uint size);

		void Reset();

	public:
		uint maxY = 0;
		uint pointerX = 0;
		uint pointerY = 0;

		Image image;
	};
}
