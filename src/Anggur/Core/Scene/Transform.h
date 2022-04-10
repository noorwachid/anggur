#pragma once

#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Matrix3.h"

namespace Anggur {
	struct Transform {
		Vector2 translation;
		float rotation;
		Vector2 scale;

		Transform(const Vector2& translation = Vector2(0.0f, 0.0f), float rotation = 0.0f, const Vector2& scale = 1.0f);

		Matrix3 toMatrix3();
	};
}