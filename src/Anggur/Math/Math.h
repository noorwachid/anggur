#pragma once

namespace Anggur {
	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Matrix2;
	struct Matrix3;
	struct Matrix4;

	struct Math {
		static const float pi;
		static const float twoPi;
		static const float infinity;
		static const float epsilon;

		static float max(float a, float b);
		static float min(float a, float b);
		static float clamp(float x, float lower = -1, float upper = 1);
		static float abs(float x);

		static float cos(float x);
		static float sin(float x);
		static float tan(float x);
		static float cot(float x);

		static float acos(float x);
		static float asin(float x);
		static float atan(float x);
		static float atan(float y, float x);

		static float pow(float x, float p);
		static float sqrt(float x);
		static float mod(float x, float d);
		static float floor(float x);
		static float ceil(float x);
		static float trunc(float x);
		static float round(float x);
		static float roundNearest(float x, float step);

		static float normalize(float x, float lower, float upper);
		static float lerp(float lower, float upper, float amount);
		static float remap(float x, float lowerA, float upperA, float lowerB, float upperB);

		static float toRadian(float degree);
		static float toDegree(float radian);

		static float isNearZero(float x, float epsilon = Math::epsilon);
	};
}
