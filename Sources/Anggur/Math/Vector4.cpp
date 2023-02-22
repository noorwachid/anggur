#include "Anggur/Math/Math.h"
#include "Anggur/Math/Vector4.h"

namespace Anggur
{
	// Constants

	const Vector4 Vector4::zero(0.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::one(1.0f, 1.0f, 1.0f, 1.0f);

	const Vector4 Vector4::unitX(1.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::unitY(0.0f, 1.0f, 0.0f, 0.0f);
	const Vector4 Vector4::unitZ(0.0f, 0.0f, 1.0f, 0.0f);
	const Vector4 Vector4::unitW(0.0f, 0.0f, 0.0f, 1.0f);

	const Vector4 Vector4::transparent(0.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::white = Vector4::createHex(0xFFFFFF);
	const Vector4 Vector4::smoke = Vector4::createHex(0xFAFAFA);
	const Vector4 Vector4::gray = Vector4::createHex(0x848484);
	const Vector4 Vector4::charcoal = Vector4::createHex(0x232323);
	const Vector4 Vector4::black = Vector4::createHex(0x000000);
	const Vector4 Vector4::red = Vector4::createHex(0xFF2020);
	const Vector4 Vector4::crimson = Vector4::createHex(0xDC143C);
	const Vector4 Vector4::salmon = Vector4::createHex(0xFA8072);
	const Vector4 Vector4::orange = Vector4::createHex(0xFF8301);
	const Vector4 Vector4::yellow = Vector4::createHex(0xFFFF01);
	const Vector4 Vector4::chartreuse = Vector4::createHex(0xADFC3C);
	const Vector4 Vector4::lime = Vector4::createHex(0x12FF12);
	const Vector4 Vector4::olive = Vector4::createHex(0x98BF64);
	const Vector4 Vector4::green = Vector4::createHex(0x10DA14);
	const Vector4 Vector4::seafoam = Vector4::createHex(0x3CEC97);
	const Vector4 Vector4::aquamarine = Vector4::createHex(0x7FFFD4);
	const Vector4 Vector4::cyan = Vector4::createHex(0x00FFFF);
	const Vector4 Vector4::sky = Vector4::createHex(0x2081F4);
	const Vector4 Vector4::blue = Vector4::createHex(0x2424FF);
	const Vector4 Vector4::violet = Vector4::createHex(0x7D0CE2);
	const Vector4 Vector4::purple = Vector4::createHex(0xD351FA);
	const Vector4 Vector4::grape = Vector4::createHex(0xA10066);
	const Vector4 Vector4::burgundy = Vector4::createHex(0x800020);
	const Vector4 Vector4::fuchsia = Vector4::createHex(0xF81895);
	const Vector4 Vector4::pink = Vector4::createHex(0xFFB1DD);
	const Vector4 Vector4::beige = Vector4::createHex(0xF5F5DC);
	const Vector4 Vector4::ivory = Vector4::createHex(0xFFFFF0);
	const Vector4 Vector4::brown = Vector4::createHex(0xA52A2A);
	const Vector4 Vector4::chocolate = Vector4::createHex(0xD2691E);
	const Vector4 Vector4::sandy = Vector4::createHex(0xF4A460);

	// Initializers

	Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f){};

	Vector4::Vector4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar)
	{
	}

	Vector4::Vector4(float newX, float newY, float newZ, float newW) : x(newX), y(newY), z(newZ), w(newW)
	{
	}

	// 2d class initializers

	Vector4 Vector4::createHex(uint64_t hex)
	{
		Vector4 v;

		v.x = (hex >> 16 & 0xFF) / 255.f;
		v.y = (hex >> 8 & 0xFF) / 255.f;
		v.z = (hex & 0xFF) / 255.f;
		v.w = 1.0f;

		return v;
	}

	// Casters

	const float* Vector4::ToPointer() const
	{
		return &x;
	}

	float* Vector4::ToPointer()
	{
		return &x;
	}

	std::string Vector4::ToString()
	{
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w);
	}

	// Getters

	float Vector4::GetLengthSquared() const
	{
		return (x * x + y * y + z * z + w * w);
	}

	float Vector4::GetLength() const
	{
		return (Math::Sqrt(GetLengthSquared()));
	}

	// Setters

	void Vector4::Set(float newX, float newY, float newZ, float newW)
	{
		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

	// 2nd class manipulations

	float Vector4::Dot(const Vector4& a, const Vector4& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
	}

	Vector4 Vector4::Cross(const Vector4& a, const Vector4& b)
	{
		// TODO: implement this
		return Vector4();
	}

	Vector4 Vector4::Normalize(const Vector4& a)
	{
		float length = a.GetLength();

		if (length == 0)
			return Vector4::zero;

		return Vector4(a.x / length, a.y / length, a.z / length, a.w / length);
	}

	Vector4 Vector4::Lerp(const Vector4& a, const Vector4& b, float amount)
	{
		return Vector4(Math::Lerp(a.x, b.x, amount), Math::Lerp(a.y, b.y, amount), Math::Lerp(a.z, b.z, amount),
					   Math::Lerp(a.w, b.w, amount));
	}

	// 3rd class manipulations

	Vector4 operator+(const Vector4& a, const Vector4& b)
	{
		return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}

	Vector4 operator-(const Vector4& a, const Vector4& b)
	{
		return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}

	Vector4 operator*(const Vector4& a, const Vector4& b)
	{
		return Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
	}

	Vector4 operator+(float a, const Vector4& b)
	{
		return Vector4(a + b.x, a + b.y, a + b.z, a + b.w);
	}

	Vector4 operator-(float a, const Vector4& b)
	{
		return Vector4(a - b.x, a - b.y, a - b.z, a + b.w);
	}

	Vector4 operator*(float a, const Vector4& b)
	{
		return Vector4(a * b.x, a * b.y, a * b.z, a + b.w);
	}

	Vector4& operator+=(Vector4& a, const Vector4& b)
	{
		a = a + b;
		return a;
	}

	Vector4& operator-=(Vector4& a, const Vector4& b)
	{
		a = a - b;
		return a;
	}

	Vector4& operator*=(Vector4& a, const Vector4& b)
	{
		a = a * b;
		return a;
	}

	Vector4& operator+=(Vector4& a, float b)
	{
		a = b + a;
		return a;
	}

	Vector4& operator-=(Vector4& a, float b)
	{
		a = b - a;
		return a;
	}

	Vector4& operator*=(Vector4& a, float b)
	{
		a = b * a;
		return a;
	}
}
