#pragma once

#include "Math.hpp"
#include "Random.hpp"

namespace Anggur {

struct Vector
{
    float X;
    float Y;

    Vector();
    Vector(float v);
    Vector(float X, float Y);

    const float* ToFloatPtr() const;
    float* ToFloatPtr();

    void Set(float v);
    void Set(float X, float Y);
    void SetPolar(float angle, float length);

    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector operator*(const Vector& v) const;

    Vector operator+(float scalar) const;
    Vector operator-(float scalar) const;
    Vector operator*(float scalar) const;
    Vector operator/(float scalar) const;
    Vector operator*(const Matrix& m) const;

    Vector operator-() const;

    Vector& operator+=(float scalar);
    Vector& operator-=(float scalar);
    Vector& operator*=(float scalar);
    Vector& operator/=(float scalar);

    Vector& operator+=(const Vector& v);
    Vector& operator-=(const Vector& v);
    Vector& operator*=(const Matrix& m);

    float GetLengthSq() const;
    float GetLength() const;
    float GetAngle() const;

    Vector GetPerpen() const;

    Vector& SetLengthLimit(float X);
    Vector& SetLength(float X);
    Vector& SetAngle(float theta);
    Vector& Normalize();

    static Vector Normalize(const Vector& vec);
    static Vector Lerp(const Vector& a, const Vector& b, float amount);

    static float Dot(const Vector& a, const Vector& b);
    static float Cross(const Vector& a, const Vector& b);


    static float GetLength(const Vector& a, const Vector& b);
    static float GetLengthSq(const Vector& a, const Vector& b);
    static float GetAngle(const Vector& a, const Vector& b);

    static float GetDist(const Vector& a, const Vector& b);
    static float GetDistSq(const Vector& a, const Vector& b);

    static Vector CreatePolar(float angle, float length);

    static Vector Zero;
    static Vector One;
};

}
