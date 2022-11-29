#pragma once

#include <string>

namespace Anggur
{
    struct Matrix3;

    struct Vector2
    {
        // Fields

        float x;
        float y;

        // Constants

        static Vector2 zero;
        static Vector2 one;

        static Vector2 unitX;
        static Vector2 unitY;

        // Initializers

        Vector2();

        Vector2(float scalar);

        Vector2(float newX, float newY);

        // 2nd class initializer

        static Vector2 CreatePolar(float length, float angle);

        // Casters

        const float* ToPointer() const;

        float* ToPointer();

        std::string ToString() const;

        // Getters

        float GetLengthSquared() const;

        float GetLength() const;

        float GetAngle() const;

        Vector2 GetPerpendicular() const;

        // Setters

        void Set(float scalar);

        void Set(float newX, float newY);

        void SetPolar(float length, float angle);

        void SetLength(float length);

        void SetLengthLimit(float length);

        void SetAngle(float angle);

        // 2nd class manipulations

        static float Dot(const Vector2& a, const Vector2& b);

        static float Cross(const Vector2& a, const Vector2& b);

        static Vector2 Normalize(const Vector2& a);

        static Vector2 Lerp(const Vector2& a, const Vector2& b, float amount);

        // 3rd class manipulations

        friend Vector2 operator-(const Vector2& a);

        friend Vector2 operator+(const Vector2& a, const Vector2& b);

        friend Vector2 operator-(const Vector2& a, const Vector2& b);

        friend Vector2 operator*(const Vector2& a, const Vector2& b);

        friend Vector2 operator+(float a, const Vector2& b);

        friend Vector2 operator-(float a, const Vector2& b);

        friend Vector2 operator*(float a, const Vector2& b);

        friend Vector2& operator+=(Vector2& a, const Vector2& b);

        friend Vector2& operator-=(Vector2& a, const Vector2& b);

        friend Vector2& operator*=(Vector2& a, const Vector2& b);

        friend Vector2& operator+=(Vector2& a, float b);

        friend Vector2& operator-=(Vector2& a, float b);

        friend Vector2& operator*=(Vector2& a, float b);
    };
}
