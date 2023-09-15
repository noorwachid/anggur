#pragma once

#include "anggur/math.h"
#include "anggur/math/vector2.h"

#include <string>

namespace Anggur {
	struct Matrix3 {
		float a, b, c;
		float d, e, f;
		float g, h, i;

		Matrix3();

		Matrix3(
			float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH, float newI
		);

		void set(
			float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH, float newI
		);

		const float* toPointer() const;

		std::string toString() const;

		float getDeterminant() const;

		Matrix3 getCofactor() const;

		Matrix3 getAdjugate() const;

		Matrix3 getInverse() const;

		Matrix3 getTranspose() const;

		static Matrix3 createOrthographic(const Vector2& origin, const Vector2& viewportSize, float scale = 1.0f);

		static Matrix3 createPixelPerfect(const Vector2& origin, const Vector2& viewportSize);

		static Matrix3 createTranslation(const Vector2& translation);

		static Matrix3 createRotation(float rotation);

		static Matrix3 createScale(const Vector2& scale);

		friend Matrix3 operator*(float a, const Matrix3& b);

		friend Matrix3 operator*(const Matrix3& a, const Matrix3& b);

		friend Vector2 operator*(const Matrix3& a, const Vector2& b);
	};
}
