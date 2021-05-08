#pragma once

namespace Anggur {

struct Math
{
    static const float Pi;
    static const float TwoPi;
    static const float Infinity;
    static const float Epsilon;

    template <typename T>
    static T Max(const T& a, const T& b)
    {
        return (a < b ? b : a);
    }

    template <typename T>
    static T Min(const T& a, const T& b)
    {
        return (a < b ? a : b);
    }

    template <typename T>
    static T Clamp(const T& x, const T& lower, const T& upper)
    {
        return Min(upper, Max(lower, x));
    }

    static float Clamp(float x);

    static float Abs(float x);
    static float Cos(float x);
    static float Sin(float x);
    static float Tan(float x);
    static float Acos(float x);
    static float Asin(float x);
    static float Atan(float x);
    static float Atan(float y, float x);
    static float Cot(float x);
    static float Pow(float x, float p);
    static float Sqrt(float x);
    static float Mod(float x, float d);
    static float Floor(float x);
    static float Ceil(float x);
    static float Trunc(float x);
    static float Round(float x);
    static float RoundNearest(float x, float step);

    static float Normalize(float x, float lower, float upper);
    static float Lerp(float lower, float upper, float amount);
    static float Remap(float x, float lowerA, float upperA, float lowerB, float upperB);

    static float ToRadian(float degree);
    static float ToDegree(float radian);

    static float IsNearZero(float x, float Epsilon = Math::Epsilon);
};

struct Vector;
struct Matrix;

}
