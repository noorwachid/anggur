#pragma once

#include "Math.h"
#include <string>

namespace Anggur 
{
	struct Matrix4;

	struct Vector3 
	{
		float x;
		float y;
		float z;

		Vector3(): x(0.0f), y(0.0f), z(0.0f) 
		{
		}

		Vector3(float x, float y, float z = 1.0f): x(x), y(y), z(z) 
		{
		}

		void Set(float newX, float newY, float newZ);

		float GetLengthSquared() const;

		float GetLength() const;

		std::string ToString();

		static Vector3 CreateScalar(float value);

		static float Dot(const Vector3& a, const Vector3& b);

		static Vector3 Cross(const Vector3& a, const Vector3& b);

		static Vector3 Normalize(const Vector3& a);
		
		static Vector3 Lerp(const Vector3& a, const Vector3& b, float amount);

		friend Vector3 operator+ (const Vector3& a, const Vector3& b);

		friend Vector3 operator- (const Vector3& a, const Vector3& b);

		friend Vector3 operator* (const Vector3& a, const Vector3& b);

		friend Vector3 operator* (float scalar, const Vector3& vector);

		static Vector3 zero;
		static Vector3 one;

		static Vector3 unitX;
		static Vector3 unitY;
		static Vector3 unitZ;
	};
}