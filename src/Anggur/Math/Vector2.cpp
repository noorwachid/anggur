#include "Math.h"
#include "Vector2.h"
#include "Matrix3.h"

namespace Anggur 
{
	Vector2 Vector2::zero(0, 0);
	Vector2 Vector2::one(1, 1);

	const float* Vector2::ToPointer() const 
	{
		return &x;
	}

	float* Vector2::ToPointer() 
	{
		return &x;
	}

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

	Vector2 Vector2::operator+(const Vector2& v) const 
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 Vector2::operator-(const Vector2& v) const 
	{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 Vector2::operator*(const Vector2& v) const 
	{
		return Vector2(x * v.x, y * v.y);
	}

	Vector2 Vector2::operator+(float scalar) const 
	{
		return Vector2(x + scalar, y + scalar);
	}

	Vector2 Vector2::operator-(float scalar) const 
	{
		return Vector2(x - scalar, y - scalar);
	}

	Vector2 Vector2::operator*(float scalar) const 
	{
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 Vector2::operator/(float scalar) const 
	{
		return Vector2(x / scalar, y / scalar);
	}

	Vector2 Vector2::operator-() const 
	{
		return Vector2(-x, -y);
	}

	Vector2& Vector2::operator+=(float scalar) 
	{
		x += scalar;
		y += scalar;
		return *this;
	}

	Vector2& Vector2::operator-=(float scalar) 
	{
		x -= scalar;
		y -= scalar;
		return *this;
	}

	Vector2& Vector2::operator*=(float scalar) 
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2& Vector2::operator/=(float scalar) 
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vector2& Vector2::operator+=(const Vector2& other) 
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& other) 
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	float Vector2::GetLengthSquared() const 
	{
		return (x * x + y * y);
	}

	float Vector2::GetLength() const 
	{
		return Math::Sqrt(GetLengthSquared());
	}

	Vector2& Vector2::SetLength(float x) 
	{
		Normalize();
		*this *= x;
		return *this;
	}

	float Vector2::GetAngle() const 
	{
		return Math::Atan(y, x);
	}

	Vector2& Vector2::SetAngle(float theta) 
	{
		SetPolar(theta, GetLength());
		return *this;
	}

	Vector2 Vector2::GetPerpendicular() const 
	{
		return Vector2(-y, x);
	}

	Vector2& Vector2::SetLengthLimit(float x) 
	{
		if (GetLengthSquared() > (x * x))
			return SetLength(x);
		return *this;
	}

	Vector2& Vector2::Normalize() 
	{
		float t = GetLength();
		if (t == 0) return *this;
		x /= t;
		y /= t;
		return *this;
	}

	float Vector2::Dot(const Vector2& a, const Vector2& b) 
	{
		return a.x * b.x + a.y * b.y;
	}

	float Vector2::Cross(const Vector2& a, const Vector2& b) 
	{
		return a.x * b.y - a.y * b.x;
	}

	Vector2 Vector2::Normalize(const Vector2& vec) 
	{
		Vector2 temp = vec;
		temp.Normalize();
		return temp;
	}

	Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float amount) 
	{
		return Vector2(a + (b - a) * amount);
	}

	float Vector2::GetLength(const Vector2& a, const Vector2& b) 
	{
		return Math::Sqrt(GetLengthSquared(a, b));
	}

	float Vector2::GetLengthSquared(const Vector2& a, const Vector2& b) 
	{
		float pa = a.x - b.x;
		float pb = a.y - b.y;

		return pa * pa + pb * pb;
	}

	float Vector2::GetAngle(const Vector2& a, const Vector2& b) 
	{
		return Math::Acos(Vector2::Dot(a, b));
	}

	float Vector2::GetDistance(const Vector2& a, const Vector2& b) 
	{
		return Math::Sqrt(GetDistanceSquared(a, b));
	}

	float Vector2::GetDistanceSquared(const Vector2& a, const Vector2& b) 
	{
		Vector2 c = a - b;
		return (c.x * c.x) + (c.y * c.y);
	}

	Vector2 Vector2::CreatePolar(float length, float angle) 
	{
		Vector2 temp;
		temp.SetPolar(length, angle);
		return temp;
	}
}
