#pragma once

namespace Anggur
{
    struct Vector2;
    struct Vector3;
    struct Vector4;

    struct Matrix2;
    struct Matrix3;
    struct Matrix4;

    struct Math
    {
        // Constants

        static const float pi;
        static const float twoPi;
        static const float infinity;
        static const float epsilon;

        // Casters

        static float degreeToRadian(float x);

        static float radianToDegree(float x);

        // Conditional manipulations

        static float max(float a, float b);

        static float min(float a, float b);

        static float clamp(float x, float lower = -1, float upper = 1);

        static float abs(float x);

        static float isNearZero(float x, float epsilon = ::Anggur::Math::epsilon);

        // Mapping manipulations

        static float normalize(float x, float lower, float upper);

        static float lerp(float lower, float upper, float amount);

        static float map(float x, float lowerA, float upperA, float lowerB, float upperB);

        // Floating point manipulations

        static float mod(float x, float d);

        static float floor(float x);

        static float ceil(float x);

        static float truncate(float x);

        static float round(float x);

        static float roundNearest(float x, float step);

        // Exponent manipulations

        static float pow(float x, float p);

        static float sqrt(float x);

        // Trigonometry manipulations

        static float sin(float x);

        static float tan(float x);

        static float sec(float x);

        static float cos(float x);

        static float cot(float x);

        static float csc(float x);

        // Trigonometry arc manipulations

        static float asin(float x);

        static float atan(float x);

        static float atan(float y, float x);

        static float asec(float x);

        static float acos(float x);

        static float acot(float x);

        static float acsc(float x);
    };
}
