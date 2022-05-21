#pragma once

#include <initializer_list>
#include <string>
#include <Anggur/Math/Common.h>
#include <Anggur/Math/Vector3.h>

namespace Anggur {
    struct Matrix4 {
        float a, b, c, d;
        float e, f, g, h;
        float i, j, k, l;
        float m, n, o, p;

        Matrix4() {
            set({
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f,
            });
        }

        Matrix4(const std::vector<float>& cells) {
            set(cells);
        }

        void set(const std::vector<float>& cells) {
            a = cells[0]; b = cells[1]; c = cells[2]; d = cells[3];
            e = cells[4]; f = cells[5]; g = cells[6]; h = cells[7];
            i = cells[8]; j = cells[9]; k = cells[10]; l = cells[11];
            m = cells[12]; n = cells[13]; o = cells[14]; p = cells[15];
        }

        const float* toPointer() const {
            return &a;
        }

        std::string toString() {
            std::string buffer;

            buffer += std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + ", " + std::to_string(d) + "\n";
            buffer += std::to_string(e) + ", " + std::to_string(f) + ", " + std::to_string(g) + ", " + std::to_string(h) + "\n";
            buffer += std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + ", " + std::to_string(l) + "\n";
            buffer += std::to_string(m) + ", " + std::to_string(n) + ", " + std::to_string(o) + ", " + std::to_string(p) + "\n";

            return buffer;
        }

        static Matrix4 createTranslation(const Vector3& translation) {
            return Matrix4({
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                translation.x, translation.y, translation.z, 1.0f,
            });
        }

        static Matrix4 createRotation(const Vector3& rotation) {
            return Matrix4();
        }

        static Matrix4 createRotationZ(float rotation) {
            return Matrix4({
                Math::cos(rotation), -Math::sin(rotation), 0.0f, 0.0f,
                Math::sin(rotation), Math::cos(rotation), 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f,
            });
        }

        static Matrix4 createScale(const Vector3& scale) {
            return Matrix4({
                scale.x, 0.0f, 0.0f, 0.0f,
                0.0f, scale.y, 0.0f, 0.0f,
                0.0f, 0.0f, scale.z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f,
            });
        }

        friend Matrix4 operator*(const Matrix4& l, const Matrix4& r) {
            return Matrix4({
                l.a * r.a + l.b * r.e + l.c * r.i + l.d * r.m,  l.a * r.b + l.b * r.f + l.c * r.j + l.d * r.n,  l.a * r.c + l.b * r.g + l.c * r.k + l.d * r.o,  l.a * r.d + l.b * r.h + l.c * r.l + l.d * r.p, 
                l.e * r.a + l.f * r.e + l.g * r.i + l.h * r.m,  l.e * r.b + l.f * r.f + l.g * r.j + l.h * r.n,  l.e * r.c + l.f * r.g + l.g * r.k + l.h * r.o,  l.e * r.d + l.f * r.h + l.g * r.l + l.h * r.p, 
                l.i * r.a + l.j * r.e + l.k * r.i + l.l * r.m,  l.i * r.b + l.j * r.f + l.k * r.j + l.l * r.n,  l.i * r.c + l.j * r.g + l.k * r.k + l.l * r.o,  l.i * r.d + l.j * r.h + l.k * r.l + l.l * r.p, 
                l.m * r.a + l.n * r.e + l.o * r.i + l.p * r.m,  l.m * r.b + l.n * r.f + l.o * r.j + l.p * r.n,  l.m * r.c + l.n * r.g + l.o * r.k + l.p * r.o,  l.m * r.d + l.n * r.h + l.o * r.l + l.p * r.p, 
            });
        }
    };
}