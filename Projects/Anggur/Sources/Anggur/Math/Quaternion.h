#pragma once

#include "Vector3.h"

namespace Anggur
{
	struct Quaternion
	{
		// Fields

		float x;
		float y;
		float z;
		float w;

		// Initializer

		Quaternion();

		explicit Quaternion(float newX, float newY, float newZ, float newW);

		explicit Quaternion(const Vector3& axis, float angle);

		// Getters

		float GetLengthSquared();

		float GetLength();

		// Setters

		void Set(float inX, float inY, float inZ, float inW);

		void Set(const Vector3& axis, float angle);

		// 1st class manipulations

		void Conjugate();

		// 2nd class manipulations

		static float Dot(const Quaternion& a, const Quaternion& b);

		static Quaternion Normalize(const Quaternion& a);

		static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float amount);

		static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float amount);

		static Quaternion Concatenate(const Quaternion& a, const Quaternion& b);
	};
}