#pragma once

namespace Anggur {
	
	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Matrix2;
	struct Matrix3;
	struct Matrix4;

	namespace Math {
		extern const float pi;
		extern const float twoPi;
		extern const float infinity;
		extern const float epsilon;

		float max(float a, float b);
		float min(float a, float b);
		float clamp(float x, float lower = -1, float upper = 1);
		float abs(float x);

		float cos(float x);
		float sin(float x);
		float tan(float x);
		float cot(float x);

		float acos(float x);
		float asin(float x);
		float atan(float x);
		float atan(float y, float x);

		float pow(float x, float p);
		float sqrt(float x);
		float mod(float x, float d);
		float floor(float x);
		float ceil(float x);
		float trunc(float x);
		float round(float x);
		float roundNearest(float x, float step);

		float normalize(float x, float lower, float upper);
		float lerp(float lower, float upper, float amount);
		float remap(float x, float lowerA, float upperA, float lowerB, float upperB);

		float toRadian(float degree);
		float toDegree(float radian);

		float isNearZero(float x, float epsilon = ::Anggur::Math::epsilon);
	}
}
