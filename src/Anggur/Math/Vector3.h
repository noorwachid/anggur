#pragma once

namespace Anggur {
	struct Vector3 {
		float x;
		float y;
		float z;

		static Vector3 lerp(const Vector3& a, const Vector3& b, float amount);
	};
}