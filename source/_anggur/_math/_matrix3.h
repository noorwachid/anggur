#pragma once

#include "anggur/math.h"
#include "anggur/math/vector2.h"

#include <string>

namespace Anggur
{
	struct Matrix3
	{
		float a, b, c;
		float d, e, f;
		float g, h, i;

		Matrix3();

		Matrix3(
			float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH, float newI
		);

		void Set(
			float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH, float newI
		);

		const float* ToPointer() const;

		std::string ToString() const;

		float Determinant() const;

		Matrix3 Cofactor() const;

		Matrix3 Adjugate() const;

		Matrix3 Inverse() const;

		Matrix3 Transpose() const;

		static Matrix3 CreateOrthographic(const Vector2& origin, const Vector2& viewportSize, float scale = 1.0f);

		static Matrix3 CreatePixelPerfect(const Vector2& origin, const Vector2& viewportSize);

		static Matrix3 CreateTranslation(const Vector2& translation);

		static Matrix3 CreateRotation(float rotation);

		static Matrix3 CreateScale(const Vector2& scale);

		friend Matrix3 operator*(float a, const Matrix3& b);

		friend Matrix3 operator*(const Matrix3& a, const Matrix3& b);

		friend Vector2 operator*(const Matrix3& a, const Vector2& b);
	};
}
