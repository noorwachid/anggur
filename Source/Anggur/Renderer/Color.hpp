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

    static const Color transparent;

    static const Color white;
    static const Color smoke;
    static const Color gray;
    static const Color charcoal;
    static const Color black;

    static const Color red;
    static const Color crimson;
    static const Color salmon;
    static const Color orange;
    static const Color yellow;
    static const Color chartreuse;
    static const Color lime;
    static const Color olive;
    static const Color green;
    static const Color seafoam;
    static const Color aquamarine;
    static const Color cyan;
    static const Color sky;
    static const Color blue;
    static const Color violet;
    static const Color purple;
    static const Color fuchsia;
    static const Color pink;

    static const Color beige;
    static const Color ivory;
    static const Color brown;
    static const Color chocolate;
    static const Color sandy;
};

}
