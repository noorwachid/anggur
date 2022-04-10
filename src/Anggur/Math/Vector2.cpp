#include "Vector2.h"
#include "Matrix3.h"

namespace Anggur {

	Vector2 Vector2::zero(0, 0);

	Vector2 Vector2::one(1, 1);

	Vector2::Vector2() {
		set(0.f);
	}

	Vector2::Vector2(float v) {
		set(v);
	}

	Vector2::Vector2(float x, float y) {
		set(x, y);
	}

	const float* Vector2::toPointer() const {
		return &x;
	}

	float* Vector2::toPointer() {
		return &x;
	}

	void Vector2::set(float v) {
		x = v;
		y = v;
	}

	void Vector2::set(float x, float y) {
		this->x = x;
		this->y = y;
	}

	void Vector2::setPolar(float angle, float length) {
		x = Math::cos(angle) * length;
		y = Math::sin(angle) * length;
	}

	Vector2 Vector2::operator+(const Vector2& v) const {
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 Vector2::operator-(const Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 Vector2::operator*(const Vector2& v) const {
		return Vector2(x * v.x, y * v.y);
	}

	Vector2 Vector2::operator+(float scalar) const {
		return Vector2(x + scalar, y + scalar);
	}

	Vector2 Vector2::operator-(float scalar) const {
		return Vector2(x - scalar, y - scalar);
	}

	Vector2 Vector2::operator*(float scalar) const {
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 Vector2::operator/(float scalar) const {
		return Vector2(x / scalar, y / scalar);
	}

	Vector2 Vector2::operator*(const Matrix3& m) const {
		return Vector2(
			m[0] * x + m[3] * y + m[6],
			m[1] * x + m[4] * y + m[7]
		);
	}

	Vector2 Vector2::operator-() const {
		return Vector2(-x, -y);
	}

	Vector2& Vector2::operator+=(float scalar) {
		x += scalar;
		y += scalar;
		return *this;
	}

	Vector2& Vector2::operator-=(float scalar) {
		x -= scalar;
		y -= scalar;
		return *this;
	}

	Vector2& Vector2::operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2& Vector2::operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vector2& Vector2::operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2& Vector2::operator*=(const Matrix3& m) {
		*this = *this * m;
		return *this;
	}

	float Vector2::getLengthSquared() const {
		return (x * x + y * y);
	}

	float Vector2::getLength() const {
		return Math::sqrt(getLengthSquared());
	}

	Vector2& Vector2::setLength(float x) {
		normalize();
		*this *= x;
		return *this;
	}

	float Vector2::getAngle() const {
		return Math::atan(y, x);
	}

	Vector2& Vector2::setAngle(float theta) {
		setPolar(theta, getLength());
		return *this;
	}

	Vector2 Vector2::getPerpendicular() const {
		return Vector2(-y, x);
	}

	Vector2& Vector2::setLengthLimit(float x) {
		if (getLengthSquared() > (x * x))
			return setLength(x);
		return *this;
	}

	Vector2& Vector2::normalize() {
		float t = getLength();
		if (t == 0) return *this;
		x /= t;
		y /= t;
		return *this;
	}

	float Vector2::dot(const Vector2& a, const Vector2& b) {
		return a.x * b.x + a.y * b.y;
	}

	float Vector2::cross(const Vector2& a, const Vector2& b) {
		return a.x * b.y - a.y * b.x;
	}

	Vector2 Vector2::normalize(const Vector2& vec) {
		Vector2 temp = vec;
		temp.normalize();
		return temp;
	}

	Vector2 Vector2::lerp(const Vector2& a, const Vector2& b, float amount) {
		return Vector2(a + (b - a) * amount);
	}

	float Vector2::getLength(const Vector2& a, const Vector2& b) {
		return Math::sqrt(getLengthSquared(a, b));
	}

	float Vector2::getLengthSquared(const Vector2& a, const Vector2& b) {
		float pa = a.x - b.x;
		float pb = a.y - b.y;

		return pa * pa + pb * pb;
	}

	float Vector2::getAngle(const Vector2& a, const Vector2& b) {
		return Math::acos(Vector2::dot(a, b));
	}

	float Vector2::getDistance(const Vector2& a, const Vector2& b) {
		return Math::sqrt(getDistanceSquared(a, b));
	}

	float Vector2::getDistanceSquared(const Vector2& a, const Vector2& b) {
		Vector2 c = a - b;
		return (c.x * c.x) + (c.y * c.y);
	}

	Vector2 Vector2::createPolar(float angle, float length) {
		Vector2 temp;
		temp.setPolar(angle, length);
		return temp;
	}

}
