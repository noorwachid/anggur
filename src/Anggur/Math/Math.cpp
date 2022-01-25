#include <math.h>
#include "Math.h"

namespace Anggur
{
	const float Math::pi = 3.1415926535f;

	const float Math::twoPi = Math::pi * 2;

	const float Math::infinity = INFINITY;

	const float Math::epsilon = 0.0001f;

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

	float Math::Cos(float x)
	{
		return cosf(x);
	}

	float Math::Sin(float x)
	{
		return sinf(x);
	}

	float Math::Tan(float x)
	{
		return tanf(x);
	}

	float Math::Acos(float x)
	{
		return acosf(x);
	}

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

	float Math::Cot(float x)
	{
		return 1.f / Tan(x);
	}

	float Math::Pow(float x, float p)
	{
		return powf(x, p);
	}

	float Math::Sqrt(float x)
	{
		return sqrtf(x);
	}

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

	float Math::Trunc(float x)
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

	float Math::Normalize(float x, float lower, float upper)
	{
		return (x - lower) / (upper - lower);
	}

	float Math::Lerp(float lower, float upper, float amount)
	{
		return lower + amount * (upper - lower);
	}

	float Math::Remap(float x, float lowerA, float upperA, float lowerB, float upperB)
	{
		return Lerp(lowerB, upperB, Normalize(x, lowerA, upperA));
	}

	float Math::ToRadian(float degree)
	{
		return degree * pi / 180.0f;
	}

	float Math::ToDegree(float radian)
	{
		return radian * 180.0f / pi;
	}

	float Math::IsNearZero(float x, float epsilon)
	{
		return (Abs(x) <= epsilon);
	}
}
