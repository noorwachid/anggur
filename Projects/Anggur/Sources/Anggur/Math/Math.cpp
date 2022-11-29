#include "Math.h"
#include <cmath>

namespace Anggur {
	// Constants

	const float Math::pi = 3.1415926535f;
	const float Math::twoPi = pi * 2;
	const float Math::infinity = INFINITY;
	const float Math::epsilon = 0.0001f;

	// Casters

	float Math::degreeToRadian(float x) {
		return x * pi / 180.0f;
	}

	float Math::radianToDegree(float x) {
		return x * 180.0f / pi;
	}

	// Conditional manipulations

	float Math::max(float a, float b) {
		return fmaxf(a, b);
	}

	float Math::min(float a, float b) {
		return fminf(a, b);
	}

	float Math::clamp(float x, float lower, float upper) {
		return min(upper, max(lower, x));
	}

	float Math::abs(float x) {
		return fabsf(x);
	}

	float Math::isNearZero(float x, float epsilon) {
		return (abs(x) <= epsilon);
	}

	// Mapping manipulations

	float Math::normalize(float x, float lower, float upper) {
		return (x - lower) / (upper - lower);
	}

	float Math::lerp(float lower, float upper, float amount) {
		return lower + amount * (upper - lower);
	}

	float Math::map(float x, float lowerA, float upperA, float lowerB, float upperB) {
		return lerp(lowerB, upperB, normalize(x, lowerA, upperA));
	}

	// Floating point manipulations

	float Math::mod(float x, float d) {
		return fmodf(x, d);
	}

	float Math::floor(float x) {
		return floorf(x);
	}

	float Math::ceil(float x) {
		return ceilf(x);
	}

	float Math::truncate(float x) {
		return truncf(x);
	}

	float Math::round(float x) {
		return roundf(x);
	}

	float Math::roundNearest(float x, float step) {
		return round(x / step) * step;
	}

	// Exponent manipulations

	float Math::pow(float x, float p) {
		return powf(x, p);
	}
	
	float Math::sqrt(float x) {
		return sqrtf(x);
	}

	// Trigonometry manipulations

	float Math::sin(float x) {
		return sinf(x);
	}

	float Math::tan(float x) {
		return tanf(x);
	}

	float Math::sec(float x) {
		return 0;
	}

	float Math::cos(float x) 
	{
		return cosf(x);
	}

	float Math::cot(float x) {
		return 0;
	}

	float Math::csc(float x) {
		return 0;
	}

	// Trigonometry arc manipulations

	float Math::asin(float x) {
		return asinf(x);
	}

	float Math::atan(float x) {
		return atanf(x);
	}

	float Math::atan(float y, float x) {
		return atan2f(y, x);
	}

	float Math::asec(float x) {
		return 0;
	}

	float Math::acos(float x) {
		return acosf(x);
	}

	float Math::acot(float x) {
		return 1.f / tan(x);
	}

	float Math::acsc(float x) {
		return 0;
	}
}
