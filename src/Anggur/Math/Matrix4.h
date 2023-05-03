#pragma once

#include "Anggur/Math/Math.h"
#include "Anggur/Math/Quaternion.h"
#include "Anggur/Math/Vector3.h"
#include <iostream>
#include <string>

namespace Anggur
{
	struct Matrix4
	{
		float a, b, c, d;
		float e, f, g, h;
		float i, j, k, l;
		float m, n, o, p;

		Matrix4()
			: a(1.0f), b(0.0f), c(0.0f), d(0.0f), e(0.0f), f(1.0f), g(0.0f), h(0.0f), i(0.0f), j(0.0f), k(1.0f),
			  l(0.0f), m(0.0f), n(0.0f), o(0.0f), p(1.0f)
		{
		}

		Matrix4(float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH,
				float newI, float newJ, float newK, float newL, float newM, float newN, float newO, float newP)
			: a(newA), b(newB), c(newC), d(newD), e(newE), f(newF), g(newG), h(newH), i(newI), j(newJ), k(newK),
			  l(newL), m(newM), n(newN), o(newO), p(newP)
		{
		}

		void Set(float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH,
				 float newI, float newJ, float newK, float newL, float newM, float newN, float newO, float newP)
		{
			a = newA;
			b = newB;
			c = newC;
			d = newD;
			e = newE;
			f = newF;
			c = newG;
			h = newH;
			i = newI;
			j = newJ;
			c = newK;
			l = newL;
			m = newM;
			n = newN;
			c = newO;
			p = newP;
		}

		const float* ToPointer() const
		{
			return &a;
		}

		std::string ToString()
		{
			std::string buffer;

			buffer += std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + ", " +
					  std::to_string(d) + "\n";
			buffer += std::to_string(e) + ", " + std::to_string(f) + ", " + std::to_string(g) + ", " +
					  std::to_string(h) + "\n";
			buffer += std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + ", " +
					  std::to_string(l) + "\n";
			buffer += std::to_string(m) + ", " + std::to_string(n) + ", " + std::to_string(o) + ", " +
					  std::to_string(p) + "\n";

			return buffer;
		}

		// Model Matrices

		static Matrix4 CreateTranslation(const Vector3& translation)
		{
			return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, translation.x,
						   translation.y, translation.z, 1.0f);
		}

		static Matrix4 CreateRotation(const Vector3& rotation)
		{
			return CreateRotationX(rotation.x) * CreateRotationY(rotation.y) * CreateRotationZ(rotation.z);
		}

		static Matrix4 CreateRotation(const Quaternion& q)
		{
			return Matrix4(1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z, 2.0f * q.x * q.y + 2.0f * q.w * q.z,
						   2.0f * q.x * q.z - 2.0f * q.w * q.y, 0.0f,

						   2.0f * q.x * q.y - 2.0f * q.w * q.z, 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z,
						   2.0f * q.y * q.z + 2.0f * q.w * q.x, 0.0f,

						   2.0f * q.x * q.z + 2.0f * q.w * q.y, 2.0f * q.y * q.z - 2.0f * q.w * q.x,
						   1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y, 0.0f,

						   0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 CreateRotationX(float angle)
		{
			return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, Math::Cos(angle), Math::Sin(angle), 0.0f, 0.0f,
						   -Math::Sin(angle), Math::Cos(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 CreateRotationY(float angle)
		{
			return Matrix4(Math::Cos(angle), 0.0f, -Math::Sin(angle), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, Math::Sin(angle),
						   0.0f, Math::Cos(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 CreateRotationZ(float angle)
		{
			return Matrix4(Math::Cos(angle), Math::Sin(angle), 0.0f, 0.0f, -Math::Sin(angle), Math::Cos(angle), 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 CreateScale(const Vector3& scale)
		{
			return Matrix4(scale.x, 0.0f, 0.0f, 0.0f, 0.0f, scale.y, 0.0f, 0.0f, 0.0f, 0.0f, scale.z, 0.0f, 0.0f, 0.0f,
						   0.0f, 1.0f);
		}

		// View Matrices

		static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
		{
			Vector3 f = Vector3::Normalize(target - eye);
			Vector3 s = Vector3::Normalize(Vector3::Cross(f, up));
			Vector3 u = Vector3::Normalize(Vector3::Cross(s, f));

			return Matrix4(s.x, u.x, -f.x, 0.0f, s.y, u.y, -f.y, 0.0f, s.z, u.z, -f.z, 0.0f, -Vector3::Dot(s, eye),
						   -Vector3::Dot(u, eye), Vector3::Dot(f, eye), 1.0f);
		}

		// Projection Matrices

		static Matrix4 CreatePerspective(float fovY, float width, float height, float near, float far)
		{
			float yScale = Math::Cot(fovY / 2.0f);
			float xScale = yScale * height / width;

			return Matrix4(xScale, 0.0f, 0.0f, 0.0f, 0.0f, yScale, 0.0f, 0.0f, 0.0f, 0.0f, -(far + near) / (far - near),
						   -1.0f, 0.0f, 0.0f, -2 * (near * far) / (far - near), 0.0);
		}

		static Matrix4 CreateOrthographic(float width, float height, float near, float far)
		{
			// TODO: find opengl based orthographic matrix
			return Matrix4(1.0f / width, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f / height, 0.0f, 0.0f, 0.0f, 0.0f,
						   -2.0f / (far - near), 0.0f, 0.0f, 0.0f, (far + near) / (far - near), 1.0);
		}

		friend Matrix4 operator*(const Matrix4& a, const Matrix4& b)
		{
			return Matrix4(
				a.a * b.a + a.b * b.e + a.c * b.i + a.d * b.m, a.a * b.b + a.b * b.f + a.c * b.j + a.d * b.n,
				a.a * b.c + a.b * b.g + a.c * b.k + a.d * b.o, a.a * b.d + a.b * b.h + a.c * b.l + a.d * b.p,
				a.e * b.a + a.f * b.e + a.g * b.i + a.h * b.m, a.e * b.b + a.f * b.f + a.g * b.j + a.h * b.n,
				a.e * b.c + a.f * b.g + a.g * b.k + a.h * b.o, a.e * b.d + a.f * b.h + a.g * b.l + a.h * b.p,
				a.i * b.a + a.j * b.e + a.k * b.i + a.l * b.m, a.i * b.b + a.j * b.f + a.k * b.j + a.l * b.n,
				a.i * b.c + a.j * b.g + a.k * b.k + a.l * b.o, a.i * b.d + a.j * b.h + a.k * b.l + a.l * b.p,
				a.m * b.a + a.n * b.e + a.o * b.i + a.p * b.m, a.m * b.b + a.n * b.f + a.o * b.j + a.p * b.n,
				a.m * b.c + a.n * b.g + a.o * b.k + a.p * b.o, a.m * b.d + a.n * b.h + a.o * b.l + a.p * b.p);
		}

		friend Vector3 operator*(const Matrix4& a, const Vector3& b)
		{
			return Vector3(a.a * b.x + a.e * b.y + a.i * b.z + a.m * 1.0f,
						   a.b * b.x + a.f * b.y + a.j * b.z + a.n * 1.0f,
						   a.c * b.x + a.g * b.y + a.k * b.z + a.o * 1.0f);
		}
	};
}
