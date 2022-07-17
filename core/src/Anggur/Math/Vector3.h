#pragma once

#include <string>

namespace Anggur {
	struct Matrix4;

	struct Vector3 {
		// Fields

		float x;
		float y;
		float z;

		// Constants

		static Vector3 zero;
		static Vector3 one;

		static Vector3 unitX;
		static Vector3 unitY;
		static Vector3 unitZ;

		// Initializers

		Vector3();
		
		Vector3(float scalar);
		
		Vector3(float x, float y, float z);

		// Casters

		const float* toPointer() const;

		float* toPointer();

		std::string toString();

		// Getters

		float getLengthSquared();

		float getLength();

		// Setters

		void set(float scalar);

		void set(float newX, float newY, float newZ);

		// 2nd class manipulations

		static float dot(const Vector3& a, const Vector3& b);

		static Vector3 cross(const Vector3& a, const Vector3& b);

		static Vector3 normalize(const Vector3& a);
		
		static Vector3 lerp(const Vector3& a, const Vector3& b, float amount);

		// 3rd class manipulations

		friend Vector3 operator+ (const Vector3& a, const Vector3& b);

		friend Vector3 operator- (const Vector3& a, const Vector3& b);

		friend Vector3 operator* (const Vector3& a, const Vector3& b);

		friend Vector3 operator+ (float a, const Vector3& b);

		friend Vector3 operator- (float a, const Vector3& b);

		friend Vector3 operator+ (float a, const Vector3& b);
	};
}