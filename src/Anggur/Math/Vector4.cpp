#include "Math.h"
#include "Vector4.h"

namespace Anggur 
{
	const Vector4 Vector4::transparent(0.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::white = Vector4::CreateHex(0xFFFFFF);
	const Vector4 Vector4::smoke = Vector4::CreateHex(0xFAFAFA);
	const Vector4 Vector4::gray = Vector4::CreateHex(0x848484);
	const Vector4 Vector4::charcoal = Vector4::CreateHex(0x232323);
	const Vector4 Vector4::black = Vector4::CreateHex(0x000000);
	const Vector4 Vector4::red = Vector4::CreateHex(0xFF2020);
	const Vector4 Vector4::crimson = Vector4::CreateHex(0xDC143C);
	const Vector4 Vector4::salmon = Vector4::CreateHex(0xFA8072);
	const Vector4 Vector4::orange = Vector4::CreateHex(0xFF8301);
	const Vector4 Vector4::yellow = Vector4::CreateHex(0xFFFF01);
	const Vector4 Vector4::chartreuse = Vector4::CreateHex(0xADFC3C);
	const Vector4 Vector4::lime = Vector4::CreateHex(0x12FF12);
	const Vector4 Vector4::olive = Vector4::CreateHex(0x98BF64);
	const Vector4 Vector4::green = Vector4::CreateHex(0x10DA14);
	const Vector4 Vector4::seafoam = Vector4::CreateHex(0x3CEC97);
	const Vector4 Vector4::aquamarine = Vector4::CreateHex(0x7FFFD4);
	const Vector4 Vector4::cyan = Vector4::CreateHex(0x00FFFF);
	const Vector4 Vector4::sky = Vector4::CreateHex(0x2081F4);
	const Vector4 Vector4::blue = Vector4::CreateHex(0x2424FF);
	const Vector4 Vector4::violet = Vector4::CreateHex(0x7D0CE2);
	const Vector4 Vector4::purple = Vector4::CreateHex(0xD351FA);
	const Vector4 Vector4::grape = Vector4::CreateHex(0xA10066);
	const Vector4 Vector4::burgundy = Vector4::CreateHex(0x800020);
	const Vector4 Vector4::fuchsia = Vector4::CreateHex(0xF81895);
	const Vector4 Vector4::pink = Vector4::CreateHex(0xFFB1DD);
	const Vector4 Vector4::beige = Vector4::CreateHex(0xF5F5DC);
	const Vector4 Vector4::ivory = Vector4::CreateHex(0xFFFFF0);
	const Vector4 Vector4::brown = Vector4::CreateHex(0xA52A2A);
	const Vector4 Vector4::chocolate = Vector4::CreateHex(0xD2691E);
	const Vector4 Vector4::sandy = Vector4::CreateHex(0xF4A460);

	void Vector4::Set(float newX, float newY, float newZ, float newW)
	{
		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

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

	Vector4 Vector4::CreateHex(uint64_t hex) 
	{
		Vector4 v;

		v.x = (hex >> 16 & 0xFF) / 255.f;
		v.y = (hex >> 8 & 0xFF) / 255.f;
		v.z = (hex & 0xFF) / 255.f;
		v.w = 1.0f;

		return v;
	}

	Vector4 Vector4::Lerp(const Vector4& a, const Vector4& b, float amount) 
	{
		return Vector4(
			Math::Lerp(a.x, b.x, amount),
			Math::Lerp(a.y, b.y, amount),
			Math::Lerp(a.z, b.z, amount),
			Math::Lerp(a.w, b.w, amount)
		);
	}
}
