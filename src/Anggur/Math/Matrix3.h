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

        Matrix3() 
        {
            Set({
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f,
            });
        }

        Matrix3(const std::vector<float>& cells) 
        {
            Set(cells);
        }

        void Set(const std::vector<float>& cells) 
        {
            a = cells[0]; b = cells[1]; c = cells[2]; 
			d = cells[3]; e = cells[4]; f = cells[5]; 
			g = cells[6]; h = cells[7]; i = cells[8];
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
            return Matrix3({
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                translation.x, translation.y, 1.0f,
            });
        }

        static Matrix3 CreateRotation(float rotation) 
        {
            return Matrix3({
                Math::Cos(rotation), -Math::Sin(rotation), 0.0f,
                Math::Sin(rotation), Math::Cos(rotation), 0.0f, 
                0.0f, 0.0f, 1.0f,
            });
        }

        static Matrix3 CreateScale(const Vector2& scale) 
        {
            return Matrix3({
                scale.x, 0.0f, 0.0f,
                0.0f, scale.y, 0.0f,
                0.0f, 0.0f, 0.0f,
            });
        }

        friend Matrix3 operator*(const Matrix3& l, const Matrix3& r) 
        {
            return Matrix3({
                l.a * r.a + l.b * r.d + l.c * r.g,  l.a * r.b + l.b * r.e + l.c * r.h,  l.a * r.c + l.b * r.f + l.c * r.i,
                l.d * r.a + l.e * r.d + l.f * r.g,  l.d * r.b + l.e * r.e + l.f * r.h,  l.d * r.c + l.e * r.f + l.f * r.i,
                l.g * r.a + l.h * r.d + l.i * r.g,  l.g * r.b + l.h * r.e + l.i * r.h,  l.g * r.c + l.h * r.f + l.i * r.i,
            });
        }
    };
}