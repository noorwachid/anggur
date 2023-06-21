#pragma once

namespace Anggur {
	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Matrix2;
	struct Matrix3;
	struct Matrix4;

	namespace Math {
		// Constants

		constexpr float pi = 3.1415926535f;
		constexpr float twoPi = pi * 2;
		constexpr float epsilon = 0.0001f;

		// Casters

		float degreeToRadian(float x);

		float radianToDegree(float x);

		// Conditional manipulations

		float max(float a, float b);

		float min(float a, float b);

		float clamp(float x, float lower = -1, float upper = 1);

		float abs(float x);

		float isNearZero(float x, float epsilon = ::Anggur::Math::epsilon);

		// Mapping manipulations

		float normalize(float x, float lower, float upper);

		float lerp(float lower, float upper, float amount);

		float map(float x, float lowerA, float upperA, float lowerB, float upperB);

		// Floating point manipulations

		float mod(float x, float d);

		float floor(float x);

		float ceil(float x);

		float truncate(float x);

		float round(float x);

		float roundNearest(float x, float step);

		// Exponent manipulations

		float pow(float x, float p);

		float sqrt(float x);

		// Trigonometry manipulations

		float sin(float x);

		float tan(float x);

		float sec(float x);

		float cos(float x);

		float cot(float x);

		float csc(float x);

		// Trigonometry arc manipulations

		float asin(float x);

		float atan(float x);

		float atan(float y, float x);

		float asec(float x);

		float acos(float x);

		float acot(float x);

		float acsc(float x);
	}
}
