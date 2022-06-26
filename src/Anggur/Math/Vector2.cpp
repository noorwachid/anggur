#include <Anggur/Math/Common.h>
#include <Anggur/Math/Random.h>
#include <Anggur/Math/Vector2.h>
#include <Anggur/Math/Matrix3.h>

namespace Anggur {

	Vector2 Vector2::zero(0, 0);

	Vector2 Vector2::one(1, 1);

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

	void Vector2::setPolar(float length, float angle) {
		x = cos(angle) * length;
		y = sin(angle) * length;
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
			m.a * x + m.d * y + m.g,
			m.b * x + m.e * y + m.h
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
		return sqrt(getLengthSquared());
	}

	Vector2& Vector2::setLength(float x) {
		normalize();
		*this *= x;
		return *this;
	}

	float Vector2::getAngle() const {
		return Math::Atan(y, x);
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
		return sqrt(getLengthSquared(a, b));
	}

	float Vector2::getLengthSquared(const Vector2& a, const Vector2& b) {
		float pa = a.x - b.x;
		float pb = a.y - b.y;

		return pa * pa + pb * pb;
	}

	float Vector2::getAngle(const Vector2& a, const Vector2& b) {
		return acos(Vector2::dot(a, b));
	}

	float Vector2::getDistance(const Vector2& a, const Vector2& b) {
		return sqrt(getDistanceSquared(a, b));
	}

	float Vector2::getDistanceSquared(const Vector2& a, const Vector2& b) {
		Vector2 c = a - b;
		return (c.x * c.x) + (c.y * c.y);
	}

	Vector2 Vector2::createPolar(float length, float angle) {
		Vector2 temp;
		temp.setPolar(length, angle);
		return temp;
	}

}
