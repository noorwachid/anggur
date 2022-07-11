#pragma once

#include <string>

namespace Anggur {
	struct Matrix3;

	struct Vector2 {
		float x;
		float y;

		Vector2(): x(0.0f), y(0.0f)
		{
		}

		Vector2(float scalar): x(scalar), y(scalar)
		{
		}

		Vector2(float newX, float newY): x(newX), y(newY)
		{
		}

		const float* ToPointer() const;
		float* ToPointer();

		void Set(float scalar);
		void Set(float newX, float newY);
		void SetPolar(float length, float angle);

		Vector2 operator+(const Vector2& v) const;
		Vector2 operator-(const Vector2& v) const;
		Vector2 operator*(const Vector2& v) const;

		Vector2 operator+(float scalar) const;
		Vector2 operator-(float scalar) const;
		Vector2 operator*(float scalar) const;
		Vector2 operator/(float scalar) const;

		Vector2 operator-() const;

		Vector2& operator+=(float scalar);
		Vector2& operator-=(float scalar);
		Vector2& operator*=(float scalar);
		Vector2& operator/=(float scalar);

		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);

		float GetLengthSquared() const;
		float GetLength() const;
		float GetAngle() const;

		Vector2 GetPerpendicular() const;

		Vector2& SetLengthLimit(float x);
		Vector2& SetLength(float x);
		Vector2& SetAngle(float theta);
		Vector2& Normalize();
		
		std::string ToString();

		static Vector2 Normalize(const Vector2& vec);
		static Vector2 Lerp(const Vector2& a, const Vector2& b, float amount);

		static float Dot(const Vector2& a, const Vector2& b);
		static float Cross(const Vector2& a, const Vector2& b);

		static float GetLength(const Vector2& a, const Vector2& b);
		static float GetLengthSquared(const Vector2& a, const Vector2& b);
		static float GetAngle(const Vector2& a, const Vector2& b);

		static float GetDistance(const Vector2& a, const Vector2& b);
		static float GetDistanceSquared(const Vector2& a, const Vector2& b);

		static Vector2 CreatePolar(float length, float angle);

		static Vector2 zero;
		static Vector2 one;
	};
}
