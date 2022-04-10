#pragma once

#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Matrix3.h"

namespace Anggur {
	struct Transform {
		Vector2 translation;
		float rotation;
		Vector2 scale;

		Matrix3 toMatrix3();
	};
}