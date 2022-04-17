#include <math.h>
#include <Anggur/Math/Math.h>

namespace Anggur {
	const float Math::pi = 3.1415926535f;

	const float Math::twoPi = Math::pi * 2;

	const float Math::infinity = INFINITY;

	const float Math::epsilon = 0.0001f;

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

	float Math::cos(float x) {
		return cosf(x);
	}

	float Math::sin(float x) {
		return sinf(x);
	}

	float Math::tan(float x) {
		return tanf(x);
	}

	float Math::acos(float x) {
		return acosf(x);
	}

	float Math::asin(float x) {
		return asinf(x);
	}

	float Math::atan(float x) {
		return atanf(x);
	}

	float Math::atan(float y, float x) {
		return atan2f(y, x);
	}

	float Math::cot(float x) {
		return 1.f / tan(x);
	}

	float Math::pow(float x, float p) {
		return powf(x, p);
	}

	float Math::sqrt(float x) {
		return sqrtf(x);
	}

	float Math::mod(float x, float d) {
		return fmodf(x, d);
	}

	float Math::floor(float x) {
		return floorf(x);
	}

	float Math::ceil(float x) {
		return ceilf(x);
	}

	float Math::trunc(float x) {
		return truncf(x);
	}

	float Math::round(float x) {
		return roundf(x);
	}

	float Math::roundNearest(float x, float step) {
		return round(x / step) * step;
	}

	float Math::normalize(float x, float lower, float upper) {
		return (x - lower) / (upper - lower);
	}

	float Math::lerp(float lower, float upper, float amount) {
		return lower + amount * (upper - lower);
	}

	float Math::remap(float x, float lowerA, float upperA, float lowerB, float upperB) {
		return lerp(lowerB, upperB, normalize(x, lowerA, upperA));
	}

	float Math::toRadian(float degree) {
		return degree * pi / 180.0f;
	}

	float Math::toDegree(float radian) {
		return radian * 180.0f / pi;
	}

	float Math::isNearZero(float x, float epsilon) {
		return (abs(x) <= epsilon);
	}
}
