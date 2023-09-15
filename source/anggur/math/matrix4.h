#pragma once

#include "anggur/math.h"
#include "anggur/math/quaternion.h"
#include "anggur/math/vector3.h"
#include <iostream>
#include <string>

namespace Anggur {
	struct Matrix4 {
		float a, b, c, d;
		float e, f, g, h;
		float i, j, k, l;
		float m, n, o, p;

		Matrix4();

		Matrix4(
			float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH, float newI,
			float newJ, float newK, float newL, float newM, float newN, float newO, float newP
		);

		void set(
			float newA, float newB, float newC, float newD, float newE, float newF, float newG, float newH, float newI,
			float newJ, float newK, float newL, float newM, float newN, float newO, float newP
		);

		const float* toPointer() const;

		std::string toString() const;

		// Model Matrices

		static Matrix4 createTranslation(const Vector3& translation);

		static Matrix4 createRotation(const Vector3& rotation);

		static Matrix4 createRotation(const Quaternion& q);

		static Matrix4 createRotationX(float angle);

		static Matrix4 createRotationY(float angle);

		static Matrix4 createRotationZ(float angle);

		static Matrix4 createScale(const Vector3& scale);

		// View Matrices

		static Matrix4 createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

		// Projection Matrices

		static Matrix4 createPerspective(float fovY, float width, float height, float near, float far);

		static Matrix4 createOrthographic(float width, float height, float near, float far);

		friend Matrix4 operator*(const Matrix4& a, const Matrix4& b);

		friend Vector3 operator*(const Matrix4& a, const Vector3& b);
	};
}
