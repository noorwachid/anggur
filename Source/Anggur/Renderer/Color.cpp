#include "Color.hpp"
#include <Anggur/Math/Math.hpp>

namespace Anggur {

const Color Color::transparent(0, 0, 0, 0);

const Color Color::white = Color::CreateHex(0xFFFFFF);
const Color Color::smoke = Color::CreateHex(0xFAFAFA);
const Color Color::gray = Color::CreateHex(0x848484);
const Color Color::charcoal = Color::CreateHex(0x232323);
const Color Color::black = Color::CreateHex(0x000000);

const Color Color::red = Color::CreateHex(0xFF2020);
const Color Color::crimson = Color::CreateHex(0xDC143C);
const Color Color::salmon = Color::CreateHex(0xFA8072);
const Color Color::orange = Color::CreateHex(0xFF8301);
const Color Color::yellow = Color::CreateHex(0xFFFF01);
const Color Color::chartreuse = Color::CreateHex(0xADFC3C);
const Color Color::lime = Color::CreateHex(0x12FF12);
const Color Color::olive = Color::CreateHex(0x98BF64);
const Color Color::green = Color::CreateHex(0x10DA14);
const Color Color::seafoam = Color::CreateHex(0x3CEC97);
const Color Color::aquamarine = Color::CreateHex(0x7FFFD4);
const Color Color::cyan = Color::CreateHex(0x00FFFF);
const Color Color::sky = Color::CreateHex(0x2081F4);
const Color Color::blue = Color::CreateHex(0x2424FF);
const Color Color::violet = Color::CreateHex(0x7D0CE2);
const Color Color::purple = Color::CreateHex(0xD351FA);
const Color Color::grape = Color::CreateHex(0xA10066);
const Color Color::burgundy = Color::CreateHex(0x800020);
const Color Color::fuchsia = Color::CreateHex(0xF81895);
const Color Color::pink = Color::CreateHex(0xFFB1DD);

const Color Color::beige = Color::CreateHex(0xF5F5DC);
const Color Color::ivory = Color::CreateHex(0xFFFFF0);
const Color Color::brown = Color::CreateHex(0xA52A2A);
const Color Color::chocolate = Color::CreateHex(0xD2691E);
const Color Color::sandy = Color::CreateHex(0xF4A460);

Color::Color()
{
    Set(0.0, 0.0, 0.0, 1.0);
}

Color::Color(float r, float g, float b, float a)
{
    Set(r, g, b, a);
}

Color Color::CreateHex(ulong hex)
{
    Color color;
    color.SetHex(hex);

    return color;
}

void Color::Set(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void Color::Set(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void Color::SetHex(ulong hex)
{
    r = (hex >> 16 & 0xFF) / 255.f;
    g = (hex >> 8  & 0xFF) / 255.f;
    b = (hex       & 0xFF) / 255.f;
}

const float* Color::ToFloatPtr() const
{
    return &r;
}

float* Color::ToFloatPtr()
{
    return &r;
}

Color Color::Lerp(const Color& a, const Color& b, float amount)
{
    return Color(
        Math::Lerp(a.r, b.r, amount),
        Math::Lerp(a.g, b.g, amount),
        Math::Lerp(a.b, b.b, amount),
        Math::Lerp(a.a, b.a, amount)
    );
}

}
