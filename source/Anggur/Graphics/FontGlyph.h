#include "Anggur/Math/Vector2.h"

namespace Anggur
{
	struct FontGlyph
	{
		Vector2 position;
		Vector2 size;

		size_t textureIndex = 0;

		Vector2 texturePosition;
		Vector2 textureSize;
	};
}
