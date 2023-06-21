#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Math.h"
#include "Anggur/Math/Matrix3.h"

namespace Anggur {
	// Constants

	Vector2 Vector2::zero(0.0f, 0.0f);
	Vector2 Vector2::one(1.0f, 1.0f);

	Vector2 Vector2::unitX(1.0f, 0.0f);
	Vector2 Vector2::unitY(0.0f, 1.0f);

	// Initializers

	Vector2::Vector2() : x(0.0f), y(0.0f) {
	}

	Vector2::Vector2(float scalar) : x(scalar), y(scalar) {
	}

	Vector2::Vector2(float newX, float newY) : x(newX), y(newY) {
	}

	// 2nd class initializers

	Vector2 Vector2::CreatePolar(float length, float angle) {
		Vector2 temp;
		temp.setPolar(length, angle);
		return temp;
	}

	// Casters

	const float* Vector2::toPointer() const {
		return &x;
	}

	std::string Vector2::toString() const {
		return std::to_string(x) + ", " + std::to_string(y);
	}

	// Getters

	float Vector2::getLengthSquared() const {
		return (x * x + y * y);
	}

	float Vector2::getLength() const {
		return Math::sqrt(getLengthSquared());
	}

	float Vector2::getAngle() const {
		return Math::atan(y, x);
	}

	Vector2 Vector2::getPerpendicular() const {
		return Vector2(-y, x);
	}

	// Setters

	void Vector2::set(float scalar) {
		x = scalar;
		y = scalar;
	}

	void Vector2::set(float newX, float newY) {
		x = newX;
		y = newY;
	}

	void Vector2::setPolar(float length, float angle) {
		x = Math::cos(angle) * length;
		y = Math::sin(angle) * length;
	}

	void Vector2::setLength(float length) {
		setPolar(length, getAngle());
	}

	void Vector2::setLengthLimit(float x) {
		if (getLengthSquared() > (x * x))
			setLength(x);
	}

	void Vector2::setAngle(float theta) {
		setPolar(theta, getLength());
	}

	// 2nd class manipulations

	float Vector2::dot(const Vector2& a, const Vector2& b) {
		return a.x * b.x + a.y * b.y;
	}

	float Vector2::cross(const Vector2& a, const Vector2& b) {
		return a.x * b.y - a.y * b.x;
	}

	Vector2 Vector2::normalize(const Vector2& a) {
		float length = a.getLength();

		if (length == 0)
			return Vector2::zero;

		return Vector2(a.x / length, a.y / length);
	}

	// 3rd class manipulations

	Vector2 operator-(const Vector2& a) {
		return Vector2(-a.x, -a.y);
	}

	Vector2 operator+(const Vector2& a, const Vector2& b) {
		return Vector2(a.x + b.x, a.y + b.y);
	}

	Vector2 operator-(const Vector2& a, const Vector2& b) {
		return Vector2(a.x - b.x, a.y - b.y);
	}

	Vector2 operator*(const Vector2& a, const Vector2& b) {
		return Vector2(a.x * b.x, a.y * b.y);
	}

	Vector2 operator+(float a, const Vector2& b) {
		return Vector2(a + b.x, a + b.y);
	}

	Vector2 operator-(float a, const Vector2& b) {
		return Vector2(a - b.x, a - b.y);
	}

	Vector2 operator*(float a, const Vector2& b) {
		return Vector2(a * b.x, a * b.y);
	}

	Vector2& operator+=(Vector2& a, const Vector2& b) {
		a = a + b;
		return a;
	}

	Vector2& operator-=(Vector2& a, const Vector2& b) {
		a = a - b;
		return a;
	}

	Vector2& operator*=(Vector2& a, const Vector2& b) {
		a = a * b;
		return a;
	}

	Vector2& operator+=(Vector2& a, float b) {
		a = b + a;
		return a;
	}

	Vector2& operator-=(Vector2& a, float b) {
		a = b - a;
		return a;
	}

	Vector2& operator*=(Vector2& a, float b) {
		a = b * a;
		return a;
	}

	Vector2 Math::lerp(const Vector2& a, const Vector2& b, float amount) {
		return Vector2(Math::lerp(a.x, b.x, amount), Math::lerp(a.y, b.y, amount));
	}

}
