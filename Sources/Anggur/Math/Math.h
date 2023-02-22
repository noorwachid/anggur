#pragma once

namespace Anggur
{
	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Matrix2;
	struct Matrix3;
	struct Matrix4;

	struct Math
	{
		// Constants

		static const float pi;
		static const float twoPi;
		static const float infinity;
		static const float epsilon;

		// Casters

		static float DegreeToRadian(float x);

		static float RadianToDegree(float x);

		// Conditional manipulations

		static float Max(float a, float b);

		static float Min(float a, float b);

		static float Clamp(float x, float lower = -1, float upper = 1);

		static float Abs(float x);

		static float IsNearZero(float x, float epsilon = ::Anggur::Math::epsilon);

		// Mapping manipulations

		static float Normalize(float x, float lower, float upper);

		static float Lerp(float lower, float upper, float amount);

		static float Map(float x, float lowerA, float upperA, float lowerB, float upperB);

		// Floating point manipulations

		static float Mod(float x, float d);

		static float Floor(float x);

		static float Ceil(float x);

		static float Truncate(float x);

		static float Round(float x);

		static float RoundNearest(float x, float step);

		// Exponent manipulations

		static float Pow(float x, float p);

		static float Sqrt(float x);

		// Trigonometry manipulations

		static float Sin(float x);

		static float Tan(float x);

		static float Sec(float x);

		static float Cos(float x);

		static float Cot(float x);

		static float Csc(float x);

		// Trigonometry arc manipulations

		static float Asin(float x);

		static float Atan(float x);

		static float Atan(float y, float x);

		static float Asec(float x);

		static float Acos(float x);

		static float Acot(float x);

		static float Acsc(float x);
	};
}
