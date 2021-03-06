#include "Math.h"
#include "Vector3.h"
#include "Matrix4.h"

namespace Anggur {
	// Constants

	Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
	Vector3 Vector3::one(1.0f, 1.0f, 1.0f);

	Vector3 Vector3::unitX(1.0f, 0.0f, 0.0f);
	Vector3 Vector3::unitY(0.0f, 1.0f, 0.0f);
	Vector3 Vector3::unitZ(0.0f, 0.0f, 1.0f);

	// Initializers

	Vector3::Vector3(): x(0.0f), y(0.0f), z(0.0f) {
	}

	Vector3::Vector3(float scalar): x(scalar), y(scalar), z(scalar) {
	}

	Vector3::Vector3(float x, float y, float z = 0.0f): x(x), y(y), z(z)  {
	}

	// Casters


	const float* Vector3::toPointer() const {
		return &x;
	}

	float* Vector3::toPointer() {
		return &x;
	}

	std::string Vector3::toString()  {
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}

	// Getters

	float Vector3::getLengthSquared() const {
		return (x*x + y*y + z*z);
	}

	float Vector3::getLength() const {
		return (Math::sqrt(getLengthSquared()));
	}

	// Setters

	void Vector3::set(float scalar) {
		x = scalar;
		y = scalar;
		z = scalar;
	}

	void Vector3::set(float newX, float newY, float newZ) {
		x = newX;
		y = newY;
		z = newZ;
	}

	// 2nd class manipulations

	float Vector3::dot(const Vector3& a, const Vector3& b) {
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	Vector3 Vector3::cross(const Vector3& a, const Vector3& b) {
		return Vector3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	Vector3 Vector3::normalize(const Vector3& a) {
		float length = a.getLength();

		if (length == 0) {
			return Vector3::zero;
		}

		return Vector3(a.x / length, a.y / length, a.z / length);
	}

	
	Vector3 Vector3::lerp(const Vector3& a, const Vector3& b, float amount) {
		return Vector3(
			Math::lerp(a.x, b.x, amount),
			Math::lerp(a.y, b.y, amount),
			Math::lerp(a.z, b.z, amount)
		);
	}

	// 3rd class manipulations

	Vector3 operator+ (const Vector3& a, const Vector3& b) {
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	Vector3 operator- (const Vector3& a, const Vector3& b) {
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	Vector3 operator* (const Vector3& a, const Vector3& b) {
		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
	}
	
	Vector3 operator+ (float a, const Vector3& b) {
		return Vector3(a + b.x, a + b.y, a + b.z);
	}

	Vector3 operator- (float a, const Vector3& b) {
		return Vector3(a - b.x, a - b.y, a - b.z);
	}

	Vector3 operator* (float a, const Vector3& b) {
		return Vector3(a * b.x, a * b.y, a * b.z);
	}

	Vector3& operator+= (Vector3& a, const Vector3& b) {
		a = a + b;
		return a;
	}

	Vector3& operator-= (Vector3& a, const Vector3& b) {
		a = a - b;
		return a;
	}

	Vector3& operator*= (Vector3& a, const Vector3& b) {
		a = a * b;
		return a;
	}

	Vector3& operator+= (Vector3& a, float b) {
		a = b + a;
		return a;
	}

	Vector3& operator-= (Vector3& a, float b) {
		a = b - a;
		return a;
	}

	Vector3& operator*= (Vector3& a, float b) {
		a = b * a;
		return a;
	}
}