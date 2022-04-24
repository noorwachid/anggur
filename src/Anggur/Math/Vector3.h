#pragma once

namespace Anggur {
	struct Vector3 {
		float x;
		float y;
		float z;

		Vector3(): x(1.0f), y(1.0f), z(1.0f) {
		}

		Vector3(float x, float y, float z): x(x), y(y), z(z) {
		}

		static Vector3 lerp(const Vector3& a, const Vector3& b, float amount);
	};
}