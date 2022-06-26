#pragma once

#include <initializer_list>
#include <string>
#include <vector>
#include "Math.h"
#include "Vector2.h"

namespace Anggur 
{
    struct Matrix3 
    {
        float a, b, c;
        float d, e, f;
		float g, h, i;

        Matrix3():
            a(1.0f), b(0.0f), c(0.0f),
            d(0.0f), e(1.0f), f(0.0f),
            g(0.0f), h(0.0f), i(1.0f) 
        {
        }

        Matrix3(
            float newA, float newB, float newC,
            float newD, float newE, float newF,
            float newG, float newH, float newI
        ):
             a(newA), b(newB), c(newC),
             d(newD), e(newE), f(newF),
             g(newG), h(newH), i(newI)
        {
        }

        void Set(
            float newA, float newB, float newC,
            float newD, float newE, float newF,
            float newG, float newH, float newI
        ) 
        {
            a = newA; b = newB; c = newC;
            d = newD; e = newE; g = newG;
            g = newG; h = newH; i = newI;
        }

        const float* ToPointer() const 
        {
            return &a;
        }

        std::string ToString() 
        {
            std::string buffer;

            buffer += std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + "\n";
            buffer += std::to_string(d) + ", " + std::to_string(e) + ", " + std::to_string(f) + "\n";
            buffer += std::to_string(g) + ", " + std::to_string(h) + ", " + std::to_string(i) + "\n";

            return buffer;
        }

        static Matrix3 CreateTranslation(const Vector2& translation) 
        {
            return Matrix3(
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                translation.x, translation.y, 1.0f
            );
        }

        static Matrix3 CreateRotation(float rotation) 
        {
            return Matrix3(
                Math::Cos(rotation), -Math::Sin(rotation), 0.0f,
                Math::Sin(rotation), Math::Cos(rotation), 0.0f, 
                0.0f, 0.0f, 1.0f
            );
        }

        static Matrix3 CreateScale(const Vector2& scale) 
        {
            return Matrix3(
                scale.x, 0.0f, 0.0f,
                0.0f, scale.y, 0.0f,
                0.0f, 0.0f, 0.0f
            );
        }

        friend Matrix3 operator*(const Matrix3& l, const Matrix3& r) 
        {
            return Matrix3(
                l.a * r.a + l.b * r.d + l.c * r.g,  l.a * r.b + l.b * r.e + l.c * r.h,  l.a * r.c + l.b * r.f + l.c * r.i,
                l.d * r.a + l.e * r.d + l.f * r.g,  l.d * r.b + l.e * r.e + l.f * r.h,  l.d * r.c + l.e * r.f + l.f * r.i,
                l.g * r.a + l.h * r.d + l.i * r.g,  l.g * r.b + l.h * r.e + l.i * r.h,  l.g * r.c + l.h * r.f + l.i * r.i
            );
        }

        friend Vector2 operator*=(const Vector2& l, const Matrix3& r)
        {
            return Vector2(
                r.a * l.x + r.d * l.y + r.g,
                r.b * l.x + r.e * l.y + r.h
            );
        }
    };
}