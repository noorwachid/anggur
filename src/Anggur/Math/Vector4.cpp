#include <Anggur/Math/Vector4.h>
#include <Anggur/Math/Math.h>

namespace Anggur {

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

	Vector4::Vector4() {
		set(0.0, 0.0, 0.0, 1.0);
	}

	Vector4::Vector4(float x, float y, float z, float w) {
		set(x, y, z, w);
	}

	void Vector4::set(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	const float* Vector4::toPointer() const {
		return &x;
	}

	float* Vector4::toPointer() {
		return &x;
	}

	Vector4 Vector4::createHex(uint64_t hex) {
		Vector4 v;

		v.x = (hex >> 16 & 0xFF) / 255.f;
		v.y = (hex >> 8 & 0xFF) / 255.f;
		v.z = (hex & 0xFF) / 255.f;
		v.w = 1.0f;

		return v;
	}

	Vector4 Vector4::lerp(const Vector4& a, const Vector4& b, float amount) {
		return Vector4(
			Math::lerp(a.x, b.x, amount),
			Math::lerp(a.y, b.y, amount),
			Math::lerp(a.z, b.z, amount),
			Math::lerp(a.w, b.w, amount)
		);
	}
}
