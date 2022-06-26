#pragma once

#include <string>
#include "Math.h"
#include "Vector3.h"

namespace Anggur 
{
    struct Matrix4 
    {
        float a, b, c, d;
        float e, f, g, h;
        float i, j, k, l;
        float m, n, o, p;

        Matrix4():
            a(1.0f), b(0.0f), c(0.0f), d(0.0f),
            e(0.0f), f(1.0f), g(0.0f), h(0.0f),
            i(0.0f), j(0.0f), k(1.0f), l(0.0f),
            m(0.0f), n(0.0f), o(0.0f), p(1.0f)
        {
        }

        Matrix4(
            float newA, float newB, float newC, float newD,
            float newE, float newF, float newG, float newH,
            float newI, float newJ, float newK, float newL,
            float newM, float newN, float newO, float newP
        ):
            a(newA), b(newB), c(newC), d(newD),
            e(newE), f(newF), g(newG), h(newH),
            i(newI), j(newJ), k(newK), l(newL),
            m(newM), n(newN), o(newO), p(newP)
        {
        }

        void Set(
            float newA, float newB, float newC, float newD,
            float newE, float newF, float newG, float newH,
            float newI, float newJ, float newK, float newL,
            float newM, float newN, float newO, float newP
        )
        {
            a = newA; b = newB; c = newC; d = newD;
            e = newE; f = newF; c = newG; h = newH;
            i = newI; j = newJ; c = newK; l = newL;
            m = newM; n = newN; c = newO; p = newP;
        }

        const float* ToPointer() const 
        {
            return &a;
        }

        std::string ToString() 
        {
            std::string buffer;

            buffer += std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + ", " + std::to_string(d) + "\n";
            buffer += std::to_string(e) + ", " + std::to_string(f) + ", " + std::to_string(g) + ", " + std::to_string(h) + "\n";
            buffer += std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + ", " + std::to_string(l) + "\n";
            buffer += std::to_string(m) + ", " + std::to_string(n) + ", " + std::to_string(o) + ", " + std::to_string(p) + "\n";

            return buffer;
        }

        static Matrix4 CreateTranslation(const Vector3& translation) 
        {
            return Matrix4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                translation.x, translation.y, translation.z, 1.0f
            );
        }

        static Matrix4 CreateRotation(const Vector3& rotation) 
        {
            return CreateRotationX(rotation.x) * CreateRotationY(rotation.y) * CreateRotationZ(rotation.z);
        }

        static Matrix4 CreateRotationX(float angle)
        {
            return Matrix4(
                1.0f, 0.0f, 0.0f , 0.0f,
                0.0f, Math::Cos(angle), Math::Sin(angle), 0.0f,
                0.0f, -Math::Sin(angle), Math::Cos(angle), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        static Matrix4 CreateRotationY(float angle)
        {
            return Matrix4(
                Math::Cos(angle), 0.0f, -Math::Sin(angle), 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                Math::Sin(angle), 0.0f, Math::Cos(angle), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        static Matrix4 CreateRotationZ(float angle) 
        {
            return Matrix4(
                Math::Cos(angle), Math::Sin(angle), 0.0f, 0.0f,
                -Math::Sin(angle), Math::Cos(angle), 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        static Matrix4 CreateScale(const Vector3& scale) 
        {
            return Matrix4(
                scale.x, 0.0f, 0.0f, 0.0f,
                0.0f, scale.y, 0.0f, 0.0f,
                0.0f, 0.0f, scale.z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        friend Matrix4 operator*(const Matrix4& l, const Matrix4& r) 
        {
            return Matrix4(
                l.a * r.a + l.b * r.e + l.c * r.i + l.d * r.m,  l.a * r.b + l.b * r.f + l.c * r.j + l.d * r.n,  l.a * r.c + l.b * r.g + l.c * r.k + l.d * r.o,  l.a * r.d + l.b * r.h + l.c * r.l + l.d * r.p, 
                l.e * r.a + l.f * r.e + l.g * r.i + l.h * r.m,  l.e * r.b + l.f * r.f + l.g * r.j + l.h * r.n,  l.e * r.c + l.f * r.g + l.g * r.k + l.h * r.o,  l.e * r.d + l.f * r.h + l.g * r.l + l.h * r.p, 
                l.i * r.a + l.j * r.e + l.k * r.i + l.l * r.m,  l.i * r.b + l.j * r.f + l.k * r.j + l.l * r.n,  l.i * r.c + l.j * r.g + l.k * r.k + l.l * r.o,  l.i * r.d + l.j * r.h + l.k * r.l + l.l * r.p, 
                l.m * r.a + l.n * r.e + l.o * r.i + l.p * r.m,  l.m * r.b + l.n * r.f + l.o * r.j + l.p * r.n,  l.m * r.c + l.n * r.g + l.o * r.k + l.p * r.o,  l.m * r.d + l.n * r.h + l.o * r.l + l.p * r.p
            );
        }

        friend Vector3 operator*(const Vector3& l, const Matrix4& r)
        {
            return Vector3(
                r.a * l.x + r.e * l.y + r.i + r.d * l.z + r.m,
                r.b * l.x + r.f * l.y + r.j + r.e * l.z + r.n,
                r.c * l.x + r.g * l.y + r.k + r.f * l.z + r.o
            );
        }
    };
}