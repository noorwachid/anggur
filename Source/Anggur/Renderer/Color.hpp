#pragma once

#include <Anggur/Helper/Primitive.hpp>

namespace Anggur {

struct Color
{
    float r;
    float g;
    float b;
    float a;

    Color();
    Color(float r, float g, float b, float a = 1.0);

    static Color CreateHex(uint hex);

    void Set(float r, float g, float b);
    void SetHex(uint hex);

    const float* ToFloatPtr() const;
    float* ToFloatPtr();

    /*
    void SetHue(float h);
    void SetSaturation(float s);
    void SetValue(float v);
    void SetBrightness(float v);

    void GetHue(float h);
    void GetSaturation(float s);
    void GetValue(float v);
    void GetBrightness(float v);
    */

    static Color Lerp(const Color& a, const Color& b, float amount);

    static const Color White;
    static const Color Smoke;
    static const Color Gray;
    static const Color Charcoal;
    static const Color Black;

    static const Color Red;
    static const Color Crimson;
    static const Color Salmon;
    static const Color Orange;
    static const Color Yellow;
    static const Color Chartreuse;
    static const Color Lime;
    static const Color Olive;
    static const Color Green;
    static const Color Seafoam;
    static const Color Aquamarine;
    static const Color Cyan;
    static const Color Blue;
    static const Color Purple;

    static const Color Beige;
    static const Color Ivory;
    static const Color Brown;
    static const Color Chocolate;
    static const Color Sandy;
};

}
