#pragma once

#include "Anggur/Math.h"
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

		Matrix4();

		Matrix4(
			float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH, float newI,
			float newJ, float newK, float newL, float newM, float newN, float newO, float newP
		);

		void Set(
			float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH, float newI,
			float newJ, float newK, float newL, float newM, float newN, float newO, float newP
		);

		const float* ToPointer() const;

		std::string ToString() const;

		// Model Matrices

		static Matrix4 CreateTranslation(const Vector3& translation);

		static Matrix4 CreateRotation(const Vector3& rotation);

		static Matrix4 CreateRotation(const Quaternion& q);

		static Matrix4 CreateRotationX(float angle);

		static Matrix4 CreateRotationY(float angle);

		static Matrix4 CreateRotationZ(float angle);

		static Matrix4 CreateScale(const Vector3& scale);

		// View Matrices

		static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

		// Projection Matrices

		static Matrix4 CreatePerspective(float fovY, float width, float height, float near, float far);

		static Matrix4 CreateOrthographic(float width, float height, float near, float far);

		friend Matrix4 operator*(const Matrix4& a, const Matrix4& b);

		friend Vector3 operator*(const Matrix4& a, const Vector3& b);
	};
}
