#pragma once

#include <initializer_list>
#include <string>
#include <vector>
#include "Math.h"
#include "Vector2.h"

namespace Anggur {
    struct Matrix3 {
        float a, b, c;
        float d, e, f;
		float g, h, i;

        Matrix3():
            a(1.0f), b(0.0f), c(0.0f),
            d(0.0f), e(1.0f), f(0.0f),
            g(0.0f), h(0.0f), i(1.0f) {
        }

        Matrix3(
            float newA, float newB, float newC,
            float newD, float newE, float newF,
            float newG, float newH, float newI
        ):
             a(newA), b(newB), c(newC),
             d(newD), e(newE), f(newF),
             g(newG), h(newH), i(newI) {
        }

        void Set(
            float newA, float newB, float newC,
            float newD, float newE, float newF,
            float newG, float newH, float newI
        ) {
            a = newA; b = newB; c = newC;
            d = newD; e = newE; g = newG;
            g = newG; h = newH; i = newI;
        }

        const float* toPointer() const {
            return &a;
        }

        std::string toString() {
            std::string buffer;

            buffer += std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + "\n";
            buffer += std::to_string(d) + ", " + std::to_string(e) + ", " + std::to_string(f) + "\n";
            buffer += std::to_string(g) + ", " + std::to_string(h) + ", " + std::to_string(i) + "\n";

            return buffer;
        }

        static Matrix3 createTranslation(const Vector2& translation) {
            return Matrix3(
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                translation.x, translation.y, 1.0f
            );
        }

        static Matrix3 createRotation(float rotation) {
            return Matrix3(
                Math::cos(rotation), -Math::sin(rotation), 0.0f,
                Math::sin(rotation), Math::cos(rotation), 0.0f, 
                0.0f, 0.0f, 1.0f
            );
        }

        static Matrix3 createScale(const Vector2& scale) {
            return Matrix3(
                scale.x, 0.0f, 0.0f,
                0.0f, scale.y, 0.0f,
                0.0f, 0.0f, 0.0f
            );
        }

        friend Matrix3 operator* (const Matrix3& a, const Matrix3& b) {
            return Matrix3(
                a.a * b.a + a.b * b.d + a.c * b.g,  a.a * b.b + a.b * b.e + a.c * b.h,  a.a * b.c + a.b * b.f + a.c * b.i,
                a.d * b.a + a.e * b.d + a.f * b.g,  a.d * b.b + a.e * b.e + a.f * b.h,  a.d * b.c + a.e * b.f + a.f * b.i,
                a.g * b.a + a.h * b.d + a.i * b.g,  a.g * b.b + a.h * b.e + a.i * b.h,  a.g * b.c + a.h * b.f + a.i * b.i
            );
        }

        friend Vector2 operator* (const Matrix3& a, const Vector2& b) {
            return Vector2(
                a.a * b.x + a.d * b.y + a.g,
                a.b * b.x + a.e * b.y + a.h
            );
        }
    };
}