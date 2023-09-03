#pragma once

#include <string>

namespace Anggur
{
	struct Matrix4;

	struct Vector3
	{
		// Fields

		float x;
		float y;
		float z;

		// Constants

		static Vector3 zero;
		static Vector3 one;

		static Vector3 unitX;
		static Vector3 unitY;
		static Vector3 unitZ;

		// Initializers

		Vector3();

		Vector3(float scalar);

		Vector3(float x, float y, float z);

		// Casters

		const float* ToPointer() const;

		std::string ToString() const;

		// Getters

		float GetLengthSquared() const;

		float GetLength() const;

		// Setters

		void Set(float scalar);

		void Set(float newX, float newY, float newZ);

		// 2nd class manipulations

		static float Dot(const Vector3& a, const Vector3& b);

		static Vector3 Cross(const Vector3& a, const Vector3& b);

		static Vector3 Normalize(const Vector3& a);

		// 3rd class manipulations

		friend Vector3 operator+(const Vector3& a, const Vector3& b);

		friend Vector3 operator-(const Vector3& a, const Vector3& b);

		friend Vector3 operator*(const Vector3& a, const Vector3& b);

		friend Vector3 operator+(float a, const Vector3& b);

		friend Vector3 operator-(float a, const Vector3& b);

		friend Vector3 operator+(float a, const Vector3& b);

		friend Vector3& operator+=(Vector3& a, const Vector3& b);

		friend Vector3& operator-=(Vector3& a, const Vector3& b);

		friend Vector3& operator*=(Vector3& a, const Vector3& b);

		friend Vector3& operator+=(Vector3& a, float b);

		friend Vector3& operator-=(Vector3& a, float b);

		friend Vector3& operator*=(Vector3& a, float b);
	};

	namespace Math
	{
		Vector3 Lerp(const Vector3& a, const Vector3& b, float amount);
	}
}
