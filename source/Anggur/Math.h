#pragma once

namespace Anggur
{
	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Matrix2;
	struct Matrix3;
	struct Matrix4;

	namespace Math
	{
		// Constants

		constexpr float pi = 3.1415926535f;
		constexpr float twoPi = pi * 2;
		constexpr float epsilon = 0.0001f;

		// Casters

		float DegreeToRadian(float x);

		float RadianToDegree(float x);

		// Conditional manipulations

		float Max(float a, float b);

		float Min(float a, float b);

		float Clamp(float x, float lower = -1, float upper = 1);

		float Abs(float x);

		float IsNearZero(float x, float epsilon = ::Anggur::Math::epsilon);

		// Mapping manipulations

		float Normalize(float x, float lower, float upper);

		float Lerp(float lower, float upper, float amount);

		float Map(float x, float lowerA, float upperA, float lowerB, float upperB);

		// Floating point manipulations

		float Mod(float x, float d);

		float Floor(float x);

		float Ceil(float x);

		float Truncate(float x);

		float Round(float x);

		float RoundNearest(float x, float step);

		// Exponent manipulations

		float Pow(float x, float p);

		float Sqrt(float x);

		// Trigonometry manipulations

		float Sin(float x);

		float Tan(float x);

		float Sec(float x);

		float Cos(float x);

		float Cot(float x);

		float Csc(float x);

		// Trigonometry arc manipulations

		float Asin(float x);

		float Atan(float x);

		float Atan(float y, float x);

		float Asec(float x);

		float Acos(float x);

		float Acot(float x);

		float Acsc(float x);
	}
}
