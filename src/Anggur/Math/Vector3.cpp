#include "Math.h"
#include "Vector3.h"
#include "Matrix4.h"

namespace Anggur 
{
	Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
	Vector3 Vector3::one(1.0f, 1.0f, 1.0f);

	Vector3 Vector3::unitX(1.0f, 0.0f, 0.0f);
	Vector3 Vector3::unitY(0.0f, 1.0f, 0.0f);
	Vector3 Vector3::unitZ(0.0f, 0.0f, 1.0f);

	void Vector3::Set(float newX, float newY, float newZ)
	{
		x = newX;
		y = newY;
		z = newZ;
	}

	float Vector3::GetLengthSquared() const
	{
		return (x*x + y*y + z*z);
	}

	float Vector3::GetLength() const
	{
		return (Math::Sqrt(GetLengthSquared()));
	}

	std::string Vector3::ToString() 
	{
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}

	Vector3 Vector3::CreateScalar(float value)
	{
		return Vector3(value, value, value);
	}

	float Vector3::Dot(const Vector3& a, const Vector3& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	Vector3 Vector3::Normalize(const Vector3& a)
	{
		float length = a.GetLength();

		return Vector3(a.x / length, a.y / length, a.z / length);
	}

	
	Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float amount) 
	{
		return Vector3(
			Math::Lerp(a.x, b.x, amount),
			Math::Lerp(a.y, b.y, amount),
			Math::Lerp(a.z, b.z, amount)
		);
	}

	Vector3 operator+ (const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	Vector3 operator- (const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	Vector3 operator* (const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	Vector3 operator* (float scalar, const Vector3& vector)
	{
		return Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
	}
}