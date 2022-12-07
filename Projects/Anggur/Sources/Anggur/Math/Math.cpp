#include "Math.h"
#include <cmath>

namespace Anggur
{
	// Constants

	const float Math::pi = 3.1415926535f;
	const float Math::twoPi = pi * 2;
	const float Math::infinity = INFINITY;
	const float Math::epsilon = 0.0001f;

	// Casters

	float Math::DegreeToRadian(float x)
	{
		return x * pi / 180.0f;
	}

	float Math::RadianToDegree(float x)
	{
		return x * 180.0f / pi;
	}

	// Conditional manipulations

	float Math::Max(float a, float b)
	{
		return fmaxf(a, b);
	}

	float Math::Min(float a, float b)
	{
		return fminf(a, b);
	}

	float Math::Clamp(float x, float lower, float upper)
	{
		return Min(upper, Max(lower, x));
	}

	float Math::Abs(float x)
	{
		return fabsf(x);
	}

	float Math::IsNearZero(float x, float epsilon)
	{
		return (Abs(x) <= epsilon);
	}

	// Mapping manipulations

	float Math::Normalize(float x, float lower, float upper)
	{
		return (x - lower) / (upper - lower);
	}

	float Math::Lerp(float lower, float upper, float amount)
	{
		return lower + amount * (upper - lower);
	}

	float Math::Map(float x, float lowerA, float upperA, float lowerB, float upperB)
	{
		return Lerp(lowerB, upperB, Normalize(x, lowerA, upperA));
	}

	// Floating point manipulations

	float Math::Mod(float x, float d)
	{
		return fmodf(x, d);
	}

	float Math::Floor(float x)
	{
		return floorf(x);
	}

	float Math::Ceil(float x)
	{
		return ceilf(x);
	}

	float Math::Truncate(float x)
	{
		return truncf(x);
	}

	float Math::Round(float x)
	{
		return roundf(x);
	}

	float Math::RoundNearest(float x, float step)
	{
		return Round(x / step) * step;
	}

	// Exponent manipulations

	float Math::Pow(float x, float p)
	{
		return powf(x, p);
	}

	float Math::Sqrt(float x)
	{
		return sqrtf(x);
	}

	// Trigonometry manipulations

	float Math::Sin(float x)
	{
		return sinf(x);
	}

	float Math::Tan(float x)
	{
		return tanf(x);
	}

	float Math::Sec(float x)
	{
		return 0;
	}

	float Math::Cos(float x)
	{
		return cosf(x);
	}

	float Math::Cot(float x)
	{
		return 0;
	}

	float Math::Csc(float x)
	{
		return 0;
	}

	// Trigonometry arc manipulations

	float Math::Asin(float x)
	{
		return asinf(x);
	}

	float Math::Atan(float x)
	{
		return atanf(x);
	}

	float Math::Atan(float y, float x)
	{
		return atan2f(y, x);
	}

	float Math::Asec(float x)
	{
		return 0;
	}

	float Math::Acos(float x)
	{
		return acosf(x);
	}

	float Math::Acot(float x)
	{
		return 1.f / Tan(x);
	}

	float Math::Acsc(float x)
	{
		return 0;
	}
}
