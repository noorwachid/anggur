#include "Vector2.h"
#include "Matrix3.h"

namespace Anggur {

	Vector2 Vector2::zero(0, 0);

	Vector2 Vector2::one(1, 1);

	Vector2::Vector2() {
		Set(0.f);
	}

	Vector2::Vector2(float v) {
		Set(v);
	}

	Vector2::Vector2(float x, float y) {
		Set(x, y);
	}

	const float* Vector2::GetData() const {
		return &x;
	}

	float* Vector2::GetData() {
		return &x;
	}

	void Vector2::Set(float v) {
		x = v;
		y = v;
	}

	void Vector2::Set(float x, float y) {
		this->x = x;
		this->y = y;
	}

	void Vector2::SetPolar(float angle, float length) {
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

	float Vector2::GetLengthSq() const {
		return (x * x + y * y);
	}

	float Vector2::GetLength() const {
		return Math::sqrt(GetLengthSq());
	}

	Vector2& Vector2::SetLength(float x) {
		Normalize();
		*this *= x;
		return *this;
	}

	float Vector2::GetAngle() const {
		return Math::atan(y, x);
	}

	Vector2& Vector2::SetAngle(float theta) {
		SetPolar(theta, GetLength());
		return *this;
	}

	Vector2 Vector2::GetPerpen() const {
		return Vector2(-y, x);
	}

	Vector2& Vector2::SetLengthLimit(float x) {
		if (GetLengthSq() > (x * x))
			return SetLength(x);
		return *this;
	}

	Vector2& Vector2::Normalize() {
		float t = GetLength();
		if (t == 0) return *this;
		x /= t;
		y /= t;
		return *this;
	}

	float Vector2::Dot(const Vector2& a, const Vector2& b) {
		return a.x * b.x + a.y * b.y;
	}

	float Vector2::Cross(const Vector2& a, const Vector2& b) {
		return a.x * b.y - a.y * b.x;
	}

	Vector2 Vector2::Normalize(const Vector2& vec) {
		Vector2 temp = vec;
		temp.Normalize();
		return temp;
	}

	Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float amount) {
		return Vector2(a + (b - a) * amount);
	}

	float Vector2::GetLength(const Vector2& a, const Vector2& b) {
		return Math::sqrt(GetLengthSq(a, b));
	}

	float Vector2::GetLengthSq(const Vector2& a, const Vector2& b) {
		float pa = a.x - b.x;
		float pb = a.y - b.y;

		return pa * pa + pb * pb;
	}

	float Vector2::GetAngle(const Vector2& a, const Vector2& b) {
		return Math::acos(Vector2::Dot(a, b));
	}

	float Vector2::GetDist(const Vector2& a, const Vector2& b) {
		return Math::sqrt(GetDistSq(a, b));
	}

	float Vector2::GetDistSq(const Vector2& a, const Vector2& b) {
		Vector2 c = a - b;
		return (c.x * c.x) + (c.y * c.y);
	}

	Vector2 Vector2::CreatePolar(float angle, float length) {
		Vector2 temp;
		temp.SetPolar(angle, length);
		return temp;
	}

}
