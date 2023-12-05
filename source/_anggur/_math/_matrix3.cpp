#include "anggur/math/matrix3.h"

namespace Anggur
{
	Matrix3::Matrix3() : a(1.0f), b(0.0f), c(0.0f), d(0.0f), e(1.0f), f(0.0f), g(0.0f), h(0.0f), i(1.0f)
	{
	}

	Matrix3::Matrix3(
		float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH, float newI
	)
		: a(newA), b(newB), c(newC), d(newD), e(newE), f(newF), g(newG), h(newH), i(newI)
	{
	}

	void Matrix3::Set(
		float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH, float newI
	)
	{
		a = newA;
		b = newB;
		c = newC;
		d = newD;
		e = newE;
		f = newF;
		g = newG;
		h = newH;
		i = newI;
	}

	const float* Matrix3::ToPointer() const
	{
		return &a;
	}

	std::string Matrix3::ToString() const
	{
		std::string buffer;

		buffer += std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + "\n";
		buffer += std::to_string(d) + ", " + std::to_string(e) + ", " + std::to_string(f) + "\n";
		buffer += std::to_string(g) + ", " + std::to_string(h) + ", " + std::to_string(i) + "\n";

		return buffer;
	}

	float Matrix3::Determinant() const
	{
		float determinantA = a * ((e * i) - (f * h));
		float determinantD = -d * ((b * i) - (h * c));
		float determinantG = g * ((b * f) - (e * c));

		return determinantA + determinantD + determinantG;
	}

	Matrix3 Matrix3::Cofactor() const
	{
		Matrix3 cofactor;
		cofactor.a = (e * i) - (f * h);
		cofactor.d = -((b * i) - (h * c));
		cofactor.g = (b * f) - (e * c);

		cofactor.b = -((d * i) - (f * g));
		cofactor.e = (a * i) - (c * g);
		cofactor.h = -((a * f) - (c * d));

		cofactor.c = (d * h) - (e * g);
		cofactor.f = -((a * h) - (b * g));
		cofactor.i = (a * e) - (b * d);

		return cofactor;
	}

	Matrix3 Matrix3::Adjugate() const
	{
		return Cofactor().Transpose();
	}

	Matrix3 Matrix3::Inverse() const
	{
		float determinant = Determinant();

		if (determinant == 0.0f)
			return Matrix3(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

		float inverseDeterminant = 1.0f / Determinant();
		Matrix3 adjugate = Adjugate();

		return inverseDeterminant * adjugate;
	}

	Matrix3 Matrix3::Transpose() const
	{
		return Matrix3(a, d, g, b, e, h, c, f, i);
	}

	Matrix3 Matrix3::CreateOrthographic(const Vector2& origin, const Vector2& viewportSize, float scale)
	{
		float inverseScale = 1 / scale;
		return Matrix3(
			inverseScale, 0.0f, 0.0f, 0.0f, -inverseScale * (viewportSize.x / viewportSize.y), 0.0f, origin.x,
			-origin.y, 1.0f
		);
	}

	Matrix3 Matrix3::CreatePixelPerfect(const Vector2& origin, const Vector2& viewportSize)
	{
		return Matrix3(
			2.0f / viewportSize.x, 0.0f, 0.0f, 0.0f, -2.0f / viewportSize.y, 0.0f, origin.x, -origin.y, 1.0f
		);
	}

	Matrix3 Matrix3::CreateTranslation(const Vector2& translation)
	{
		return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, translation.x, translation.y, 1.0f);
	}

	Matrix3 Matrix3::CreateRotation(float rotation)
	{
		return Matrix3(
			Math::Cos(rotation), Math::Sin(rotation), 0.0f, -Math::Sin(rotation), Math::Cos(rotation), 0.0f, 0.0f, 0.0f,
			1.0f
		);
	}

	Matrix3 Matrix3::CreateScale(const Vector2& scale)
	{
		return Matrix3(scale.x, 0.0f, 0.0f, 0.0f, scale.y, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix3 operator*(float a, const Matrix3& b)
	{
		return Matrix3(a * b.a, a * b.b, a * b.c, a * b.d, a * b.e, a * b.f, a * b.g, a * b.h, a * b.i);
	}

	Matrix3 operator*(const Matrix3& a, const Matrix3& b)
	{
		return Matrix3(
			a.a * b.a + a.b * b.d + a.c * b.g, a.a * b.b + a.b * b.e + a.c * b.h, a.a * b.c + a.b * b.f + a.c * b.i,
			a.d * b.a + a.e * b.d + a.f * b.g, a.d * b.b + a.e * b.e + a.f * b.h, a.d * b.c + a.e * b.f + a.f * b.i,
			a.g * b.a + a.h * b.d + a.i * b.g, a.g * b.b + a.h * b.e + a.i * b.h, a.g * b.c + a.h * b.f + a.i * b.i
		);
	}

	Vector2 operator*(const Matrix3& a, const Vector2& b)
	{
		return Vector2(a.a * b.x + a.d * b.y + a.g, a.b * b.x + a.e * b.y + a.h);
	}
}
