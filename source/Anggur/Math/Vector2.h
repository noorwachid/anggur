#pragma once

#include <string>

namespace Anggur {
	struct Matrix3;

	struct Vector2 {
		// Fields

		float x;
		float y;

		// Constants

		static Vector2 zero;
		static Vector2 one;

		static Vector2 unitX;
		static Vector2 unitY;

		// Initializers

		Vector2();

		Vector2(float scalar);

		Vector2(float newX, float newY);

		// 2nd class initializer

		static Vector2 createPolar(float length, float angle);

		// Casters

		const float* toPointer() const;

		std::string toString() const;

		// Getters

		float getLengthSquared() const;

		float getLength() const;

		float getAngle() const;

		Vector2 getPerpendicular() const;

		// Setters

		void set(float scalar);

		void set(float newX, float newY);

		void setPolar(float length, float angle);

		void setLength(float length);

		void setLengthLimit(float length);

		void setAngle(float angle);

		// 2nd class manipulations

		static float dot(const Vector2& a, const Vector2& b);

		static float cross(const Vector2& a, const Vector2& b);

		static Vector2 normalize(const Vector2& a);

		// 3rd class manipulations

		friend Vector2 operator-(const Vector2& a);

		friend Vector2 operator+(const Vector2& a, const Vector2& b);

		friend Vector2 operator-(const Vector2& a, const Vector2& b);

		friend Vector2 operator*(const Vector2& a, const Vector2& b);

		friend Vector2 operator+(float a, const Vector2& b);

		friend Vector2 operator-(float a, const Vector2& b);

		friend Vector2 operator*(float a, const Vector2& b);

		friend Vector2& operator+=(Vector2& a, const Vector2& b);

		friend Vector2& operator-=(Vector2& a, const Vector2& b);

		friend Vector2& operator*=(Vector2& a, const Vector2& b);

		friend Vector2& operator+=(Vector2& a, float b);

		friend Vector2& operator-=(Vector2& a, float b);

		friend Vector2& operator*=(Vector2& a, float b);
	};

	namespace Math {
		Vector2 lerp(const Vector2& a, const Vector2& b, float amount);
	}
}
