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

		static Vector3 CreateScalar(float value)
		{
			return Vector3(value, value, value);
		}

		static float Dot(const Vector3& a, const Vector3& b)
		{
			return (a.x * b.x + a.y * b.y + a.z * b.z);
		}

		static Vector3 Cross(const Vector3& a, const Vector3& b)
		{
			return Vector3(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			);
		}

		float GetLengthSquared() const
		{
			return (x*x + y*y + z*z);
		}

		float GetLength() const
		{
			return (Math::Sqrt(GetLengthSquared()));
		}

		std::string ToString() 
		{
			return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
		}

		static Vector3 Normalize(const Vector3& a)
		{
			float length = a.GetLength();

			return Vector3(a.x / length, a.y / length, a.z / length);
		}

		static Vector3 Lerp(const Vector3& a, const Vector3& b, float amount);

		friend Vector3 operator+ (const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		friend Vector3 operator- (const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		friend Vector3 operator* (const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
		}

		friend Vector3 operator* (float scalar, const Vector3& vector)
		{
			return Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
		}

		static Vector3 zero;
		static Vector3 one;

		static Vector3 unitX;
		static Vector3 unitY;
		static Vector3 unitZ;
	};
}