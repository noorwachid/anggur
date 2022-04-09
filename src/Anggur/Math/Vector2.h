#pragma once

#include "Math.h"
#include "Random.h"

namespace Anggur {
	struct Vector2 {
		float x;
		float y;

		Vector2();
		Vector2(float v);
		Vector2(float x, float y);

		const float* GetData() const;
		float* GetData();

		void Set(float v);
		void Set(float x, float y);
		void SetPolar(float angle, float length);

		Vector2 operator+(const Vector2& v) const;
		Vector2 operator-(const Vector2& v) const;
		Vector2 operator*(const Vector2& v) const;

		Vector2 operator+(float scalar) const;
		Vector2 operator-(float scalar) const;
		Vector2 operator*(float scalar) const;
		Vector2 operator/(float scalar) const;
		Vector2 operator*(const Matrix3& m) const;

		Vector2 operator-() const;

		Vector2& operator+=(float scalar);
		Vector2& operator-=(float scalar);
		Vector2& operator*=(float scalar);
		Vector2& operator/=(float scalar);

		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(const Matrix3& m);

		float GetLengthSq() const;
		float GetLength() const;
		float GetAngle() const;

		Vector2 GetPerpen() const;

		Vector2& SetLengthLimit(float x);
		Vector2& SetLength(float x);
		Vector2& SetAngle(float theta);
		Vector2& Normalize();

		static Vector2 Normalize(const Vector2& vec);
		static Vector2 Lerp(const Vector2& a, const Vector2& b, float amount);

		static float Dot(const Vector2& a, const Vector2& b);
		static float Cross(const Vector2& a, const Vector2& b);

		static float GetLength(const Vector2& a, const Vector2& b);
		static float GetLengthSq(const Vector2& a, const Vector2& b);
		static float GetAngle(const Vector2& a, const Vector2& b);

		static float GetDist(const Vector2& a, const Vector2& b);
		static float GetDistSq(const Vector2& a, const Vector2& b);

		static Vector2 CreatePolar(float angle, float length);

		static Vector2 zero;
		static Vector2 one;
	};
}
