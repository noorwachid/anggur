#pragma once

namespace Anggur {
	struct Matrix3;

	struct Vector2 {
		float x;
		float y;

		Vector2();
		Vector2(float v);
		Vector2(float x, float y);

		const float* toPointer() const;
		float* toPointer();

		void set(float v);
		void set(float x, float y);
		void setPolar(float length, float angle);

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

		float getLengthSquared() const;
		float getLength() const;
		float getAngle() const;

		Vector2 getPerpendicular() const;

		Vector2& setLengthLimit(float x);
		Vector2& setLength(float x);
		Vector2& setAngle(float theta);
		Vector2& normalize();

		static Vector2 normalize(const Vector2& vec);
		static Vector2 lerp(const Vector2& a, const Vector2& b, float amount);

		static float dot(const Vector2& a, const Vector2& b);
		static float cross(const Vector2& a, const Vector2& b);

		static float getLength(const Vector2& a, const Vector2& b);
		static float getLengthSquared(const Vector2& a, const Vector2& b);
		static float getAngle(const Vector2& a, const Vector2& b);

		static float getDistance(const Vector2& a, const Vector2& b);
		static float getDistanceSquared(const Vector2& a, const Vector2& b);

		static Vector2 createPolar(float length, float angle);

		static Vector2 zero;
		static Vector2 one;
	};
}
