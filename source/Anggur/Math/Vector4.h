#pragma once

#include <string>

namespace Anggur
{
	struct Vector4
	{
		// Fields

		float x;
		float y;
		float z;
		float w;

		// Constants

		static const Vector4 zero;
		static const Vector4 one;

		static const Vector4 unitX;
		static const Vector4 unitY;
		static const Vector4 unitZ;
		static const Vector4 unitW;

		static const Vector4 transparent;
		static const Vector4 white;
		static const Vector4 smoke;
		static const Vector4 gray;
		static const Vector4 black;
		static const Vector4 charcoal;

		static const Vector4 red;
		static const Vector4 crimson;
		static const Vector4 salmon;
		static const Vector4 orange;
		static const Vector4 yellow;
		static const Vector4 chartreuse;
		static const Vector4 lime;
		static const Vector4 olive;
		static const Vector4 green;
		static const Vector4 seafoam;
		static const Vector4 aquamarine;
		static const Vector4 cyan;
		static const Vector4 sky;
		static const Vector4 blue;
		static const Vector4 violet;
		static const Vector4 purple;
		static const Vector4 grape;
		static const Vector4 burgundy;
		static const Vector4 fuchsia;
		static const Vector4 pink;

		static const Vector4 beige;
		static const Vector4 ivory;
		static const Vector4 brown;
		static const Vector4 chocolate;
		static const Vector4 sandy;

		// Initializer

		Vector4();

		Vector4(float scalar);

		Vector4(float newX, float newY, float newZ, float newW);

		// 2d class initializer

		static Vector4 CreateHex(uint64_t hex);

		// Casters

		const float* ToPointer() const;

		std::string ToString() const;

		// Getter

		float GetLengthSquared() const;

		float GetLength() const;

		// Setters

		void Set(float scalar);

		void Set(float newX, float newY, float newZ, float newW);

		// 2nd class manipulations

		static float Dot(const Vector4& a, const Vector4& b);

		static Vector4 Cross(const Vector4& a, const Vector4& b);

		static Vector4 Normalize(const Vector4& a);

		// 3rd class manipulations

		friend Vector4 operator+(const Vector4& a, const Vector4& b);

		friend Vector4 operator-(const Vector4& a, const Vector4& b);

		friend Vector4 operator*(const Vector4& a, const Vector4& b);

		friend Vector4 operator+(float a, const Vector4& b);

		friend Vector4 operator-(float a, const Vector4& b);

		friend Vector4 operator+(float a, const Vector4& b);

		friend Vector4& operator+=(Vector4& a, const Vector4& b);

		friend Vector4& operator-=(Vector4& a, const Vector4& b);

		friend Vector4& operator*=(Vector4& a, const Vector4& b);

		friend Vector4& operator+=(Vector4& a, float b);

		friend Vector4& operator-=(Vector4& a, float b);

		friend Vector4& operator*=(Vector4& a, float b);
	};

	namespace Math
	{
		Vector4 Lerp(const Vector4& a, const Vector4& b, float amount);
	}
}
