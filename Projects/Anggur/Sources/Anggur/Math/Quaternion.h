#pragma once

#include "Vector3.h"

namespace Anggur {
    struct Quaternion {
        // Fields

        float x;
        float y;
        float z;
        float w;

        // Initializer

        Quaternion();

        explicit Quaternion(float newX,  float newY, float newZ, float newW);

        explicit Quaternion(const Vector3& axis, float angle);

        // Getters

        float getLengthSquared();

        float getLength();

        // Setters

        void set(float inX, float inY, float inZ, float inW);

        void set(const Vector3& axis, float angle);

        // 1st class manipulations

        void conjugate();

        // 2nd class manipulations

        static float dot(const Quaternion& a, const Quaternion& b);

        static Quaternion normalize(const Quaternion& a);

        static Quaternion lerp(const Quaternion& a, const Quaternion& b, float amount);

        static Quaternion slerp(const Quaternion& a, const Quaternion& b, float amount);

        static Quaternion concatenate(const Quaternion& a, const Quaternion& b);
    };
}