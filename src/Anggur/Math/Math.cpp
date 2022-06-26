#include "Math.h"
#include <cmath>

namespace Anggur::Math 
{
	const float pi = 3.1415926535f;
	const float twoPi = pi * 2;
	const float infinity = INFINITY;
	const float epsilon = 0.0001f;

	float Max(float a, float b) 
	{
		return fmaxf(a, b);
	}

	float Min(float a, float b) 
	{
		return fminf(a, b);
	}

	float Clamp(float x, float lower, float upper) 
	{
		return Min(upper, Max(lower, x));
	}

	float Abs(float x) 
	{
		return fabsf(x);
	}

	float Cos(float x) 
	{
		return cosf(x);
	}

	float Sin(float x) 
	{
		return sinf(x);
	}

	float Tan(float x) 
	{
		return tanf(x);
	}

	float Acos(float x) 
	{
		return acosf(x);
	}

	float Asin(float x) 
	{
		return asinf(x);
	}

	float Atan(float x) 
	{
		return atanf(x);
	}

	float Atan(float y, float x) 
	{
		return atan2f(y, x);
	}

	float Cot(float x) 
	{
		return 1.f / Tan(x);
	}

	float Pow(float x, float p) 
	{
		return powf(x, p);
	}

	float Sqrt(float x) 
	{
		return sqrtf(x);
	}

	float Mod(float x, float d) 
	{
		return fmodf(x, d);
	}

	float Floor(float x) 
	{
		return floorf(x);
	}

	float Ceil(float x) 
	{
		return ceilf(x);
	}

	float Truncate(float x) 
	{
		return truncf(x);
	}

	float Round(float x) 
	{
		return roundf(x);
	}

	float RoundNearest(float x, float step) 
	{
		return Round(x / step) * step;
	}

	float Normalize(float x, float lower, float upper) 
	{
		return (x - lower) / (upper - lower);
	}

	float Lerp(float lower, float upper, float amount) 
	{
		return lower + amount * (upper - lower);
	}

	float Map(float x, float lowerA, float upperA, float lowerB, float upperB) 
	{
		return Lerp(lowerB, upperB, Normalize(x, lowerA, upperA));
	}

	float DegreeToRadian(float x) 
	{
		return x * pi / 180.0f;
	}

	float RadianToDegree(float x) 
	{
		return x * 180.0f / pi;
	}

	float IsNearZero(float x, float epsilon) 
	{
		return (Abs(x) <= epsilon);
	}
}
