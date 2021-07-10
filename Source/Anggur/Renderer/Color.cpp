#include "Color.hpp"
#include <Anggur/Math/Math.hpp>

namespace Anggur {

const Color Color::Transparent(0, 0, 0, 0);

const Color Color::White = Color::CreateHex(0xFFFFFF);
const Color Color::Smoke = Color::CreateHex(0xFAFAFA);
const Color Color::Gray = Color::CreateHex(0x848484);
const Color Color::Charcoal = Color::CreateHex(0x232323);
const Color Color::Black = Color::CreateHex(0x000000);

const Color Color::Red = Color::CreateHex(0xFF2020);
const Color Color::Crimson = Color::CreateHex(0xDC143C);
const Color Color::Salmon = Color::CreateHex(0xFA8072);
const Color Color::Orange = Color::CreateHex(0xFF8301);
const Color Color::Yellow = Color::CreateHex(0xFFFF01);
const Color Color::Chartreuse = Color::CreateHex(0xADFC3C);
const Color Color::Lime = Color::CreateHex(0x12FF12);
const Color Color::Olive = Color::CreateHex(0x98BF64);
const Color Color::Green = Color::CreateHex(0x10DA14);
const Color Color::Seafoam = Color::CreateHex(0x3CEC97);
const Color Color::Aquamarine = Color::CreateHex(0x7FFFD4);
const Color Color::Cyan = Color::CreateHex(0x00FFFF);
const Color Color::Sky = Color::CreateHex(0x2081F4);
const Color Color::Blue = Color::CreateHex(0x2424FF);
const Color Color::Violet = Color::CreateHex(0x7D0CE2);
const Color Color::Purple = Color::CreateHex(0xD351FA);
const Color Color::Grape = Color::CreateHex(0x810066);
const Color Color::Fuchsia = Color::CreateHex(0xF81895);
const Color Color::Pink = Color::CreateHex(0xFFB1DD);

const Color Color::Beige = Color::CreateHex(0xF5F5DC);
const Color Color::Ivory = Color::CreateHex(0xFFFFF0);
const Color Color::Brown = Color::CreateHex(0xA52A2A);
const Color Color::Chocolate = Color::CreateHex(0xD2691E);
const Color Color::Sandy = Color::CreateHex(0xF4A460);

Color::Color()
{
    Set(0.0, 0.0, 0.0, 1.0);
}

Color::Color(float r, float g, float b, float a)
{
    Set(r, g, b, a);
}

Color Color::CreateHex(uint hex)
{
    Color color;
    color.SetHex(hex);

    return color;
}

void Color::Set(float r, float g, float b)
{
    R = r;
    G = g;
    B = b;
}

void Color::Set(float r, float g, float b, float a)
{
    R = r;
    G = g;
    B = b;
    A = a;
}

void Color::SetHex(uint hex)
{
    R = (hex >> 16 & 0xFF) / 255.f;
    G = (hex >> 8  & 0xFF) / 255.f;
    B = (hex       & 0xFF) / 255.f;
}

const float* Color::ToFloatPtr() const
{
    return &R;
}

float* Color::ToFloatPtr()
{
    return &R;
}

Color Color::Lerp(const Color& a, const Color& b, float amount)
{
    return Color(
        Math::Lerp(a.R, b.R, amount),
        Math::Lerp(a.G, b.G, amount),
        Math::Lerp(a.B, b.B, amount),
        Math::Lerp(a.A, b.A, amount)
    );
}

}
