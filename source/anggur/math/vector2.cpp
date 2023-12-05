#include "anggur/math/vector2.h"
#include "anggur/math.h"
#include "anggur/math/matrix3.h"

namespace Anggur
{
	// Constants

	Vector2 Vector2::zero(0.0f, 0.0f);
	Vector2 Vector2::one(1.0f, 1.0f);

	Vector2 Vector2::unitX(1.0f, 0.0f);
	Vector2 Vector2::unitY(0.0f, 1.0f);

	// Initializers

	Vector2::Vector2() : x(0.0f), y(0.0f)
	{
	}

	Vector2::Vector2(float scalar) : x(scalar), y(scalar)
	{
	}

	Vector2::Vector2(float newX, float newY) : x(newX), y(newY)
	{
	}

	// 2nd class initializers

	Vector2 Vector2::CreatePolar(float length, float angle)
	{
		Vector2 temp;
		temp.SetPolar(length, angle);
		return temp;
	}

	// Casters

	const float* Vector2::ToPointer() const
	{
		return &x;
	}

	std::string Vector2::ToString() const
	{
		return std::to_string(x) + ", " + std::to_string(y);
	}

	// Getters

	float Vector2::LengthSquared() const
	{
		return (x * x + y * y);
	}

	float Vector2::Length() const
	{
		return Math::Sqrt(LengthSquared());
	}

	float Vector2::Angle() const
	{
		return Math::Atan(y, x);
	}

	Vector2 Vector2::Perpendicular() const
	{
		return Vector2(-y, x);
	}

	// Setters

	void Vector2::Set(float scalar)
	{
		x = scalar;
		y = scalar;
	}

	void Vector2::Set(float newX, float newY)
	{
		x = newX;
		y = newY;
	}

	void Vector2::SetPolar(float length, float angle)
	{
		x = Math::Cos(angle) * length;
		y = Math::Sin(angle) * length;
	}

	void Vector2::SetLength(float length)
	{
		SetPolar(length, Angle());
	}

	void Vector2::SetLengthLimit(float x)
	{
		if (LengthSquared() > (x * x))
			SetLength(x);
	}

	void Vector2::SetAngle(float theta)
	{
		SetPolar(theta, Length());
	}

	// 2nd class manipulations

	float Vector2::Dot(const Vector2& a, const Vector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	float Vector2::Cross(const Vector2& a, const Vector2& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	Vector2 Vector2::Normalize(const Vector2& a)
	{
		float length = a.Length();

		if (length == 0)
			return Vector2::zero;

		return Vector2(a.x / length, a.y / length);
	}

	// 3rd class manipulations

	Vector2 operator-(const Vector2& a)
	{
		return Vector2(-a.x, -a.y);
	}

	Vector2 operator+(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x + b.x, a.y + b.y);
	}

	Vector2 operator-(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x - b.x, a.y - b.y);
	}

	Vector2 operator*(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x * b.x, a.y * b.y);
	}

	Vector2 operator+(float a, const Vector2& b)
	{
		return Vector2(a + b.x, a + b.y);
	}

	Vector2 operator-(float a, const Vector2& b)
	{
		return Vector2(a - b.x, a - b.y);
	}

	Vector2 operator*(float a, const Vector2& b)
	{
		return Vector2(a * b.x, a * b.y);
	}

	Vector2& operator+=(Vector2& a, const Vector2& b)
	{
		a = a + b;
		return a;
	}

	Vector2& operator-=(Vector2& a, const Vector2& b)
	{
		a = a - b;
		return a;
	}

	Vector2& operator*=(Vector2& a, const Vector2& b)
	{
		a = a * b;
		return a;
	}

	Vector2& operator+=(Vector2& a, float b)
	{
		a = b + a;
		return a;
	}

	Vector2& operator-=(Vector2& a, float b)
	{
		a = b - a;
		return a;
	}

	Vector2& operator*=(Vector2& a, float b)
	{
		a = b * a;
		return a;
	}

	Vector2 Math::Lerp(const Vector2& a, const Vector2& b, float amount)
	{
		return Vector2(Math::Lerp(a.x, b.x, amount), Math::Lerp(a.y, b.y, amount));
	}

}
