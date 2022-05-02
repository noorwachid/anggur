#include <math.h>
#include <Anggur/Math/Common.h>

namespace Anggur::Math {

	const float pi = 3.1415926535f;
	const float twoPi = pi * 2;
	const float infinity = INFINITY;
	const float epsilon = 0.0001f;

	float max(float a, float b) {
		return fmaxf(a, b);
	}

	float min(float a, float b) {
		return fminf(a, b);
	}

	float clamp(float x, float lower, float upper) {
		return min(upper, max(lower, x));
	}

	float abs(float x) {
		return fabsf(x);
	}

	float cos(float x) {
		return cosf(x);
	}

	float sin(float x) {
		return sinf(x);
	}

	float tan(float x) {
		return tanf(x);
	}

	float acos(float x) {
		return acosf(x);
	}

	float asin(float x) {
		return asinf(x);
	}

	float atan(float x) {
		return atanf(x);
	}

	float atan(float y, float x) {
		return atan2f(y, x);
	}

	float cot(float x) {
		return 1.f / tan(x);
	}

	float pow(float x, float p) {
		return powf(x, p);
	}

	float sqrt(float x) {
		return sqrtf(x);
	}

	float mod(float x, float d) {
		return fmodf(x, d);
	}

	float floor(float x) {
		return floorf(x);
	}

	float ceil(float x) {
		return ceilf(x);
	}

	float trunc(float x) {
		return truncf(x);
	}

	float round(float x) {
		return roundf(x);
	}

	float roundNearest(float x, float step) {
		return round(x / step) * step;
	}

	float normalize(float x, float lower, float upper) {
		return (x - lower) / (upper - lower);
	}

	float lerp(float lower, float upper, float amount) {
		return lower + amount * (upper - lower);
	}

	float remap(float x, float lowerA, float upperA, float lowerB, float upperB) {
		return lerp(lowerB, upperB, normalize(x, lowerA, upperA));
	}

	float toRadian(float degree) {
		return degree * pi / 180.0f;
	}

	float toDegree(float radian) {
		return radian * 180.0f / pi;
	}

	float isNearZero(float x, float epsilon) {
		return (abs(x) <= epsilon);
	}
}
