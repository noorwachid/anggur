#include "Anggur/Integral.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur
{
	struct FontGlyph
	{
		Vector2 position;
		Vector2 size;

		usize textureIndex = 0;

		Vector2 texturePosition;
		Vector2 textureSize;
	};
}
