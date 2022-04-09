#include "Vector2.h"
#include "Matrix3.h"

namespace Anggur {

	const Matrix3 Matrix3::identity({
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
	});

	const Matrix3 Matrix3::zero({
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	});

	Matrix3::Matrix3() {
		*this = Matrix3::identity;
	}

	Matrix3::Matrix3(const std::initializer_list<float>& list) {
		Set(list);
	}

	Matrix3::Matrix3(const Vector2& translation, float rotation, const Vector2& scale) {
		*this = Matrix3::identity;
		Scale(scale);
		Rotate(rotation);
		Translate(translation);
	}

	void Matrix3::Set(const std::initializer_list<float>& list) {
		size_t i = 0;
		for (auto it = list.begin(); it != list.end(); ++it, ++i)
			_data[i] = *it;
	}

	const float* Matrix3::GetData() const {
		return _data;
	}

	float* Matrix3::GetData() {
		return _data;
	}

	float& Matrix3::operator[](size_t index) {
		return _data[index];
	}

	float Matrix3::operator[](size_t index) const {
		return _data[index];
	}

	Matrix3 Matrix3::operator*(const Matrix3& right) const {
		Matrix3 result;

		result[0] = _data[0] * right[0] + _data[3] * right[1] + _data[6] * right[2];
		result[1] = _data[1] * right[0] + _data[4] * right[1] + _data[7] * right[2];
		result[2] = _data[2] * right[0] + _data[5] * right[1] + _data[8] * right[2];

		result[3] = _data[0] * right[3] + _data[3] * right[4] + _data[6] * right[5];
		result[4] = _data[1] * right[3] + _data[4] * right[4] + _data[7] * right[5];
		result[5] = _data[2] * right[3] + _data[5] * right[4] + _data[8] * right[5];

		result[6] = _data[0] * right[6] + _data[3] * right[7] + _data[6] * right[8];
		result[7] = _data[1] * right[6] + _data[4] * right[7] + _data[7] * right[8];
		result[8] = _data[2] * right[6] + _data[5] * right[7] + _data[8] * right[8];

		return result;
	}

	Matrix3 Matrix3::operator+(const Matrix3& right) const {
		Matrix3 result;

		result[0] = _data[0] + right[0];
		result[1] = _data[1] + right[1];
		result[2] = _data[2] + right[2];

		result[3] = _data[3] + right[3];
		result[4] = _data[4] + right[4];
		result[5] = _data[5] + right[5];

		result[6] = _data[6] + right[6];
		result[7] = _data[7] + right[7];
		result[8] = _data[8] + right[8];

		return result;
	}

	Matrix3& Matrix3::operator*=(const Matrix3& right) {
		*this = *this * right;
		return *this;
	}

	Matrix3& Matrix3::operator+=(const Matrix3& right) {
		*this = *this + right;
		return *this;
	}

	Matrix3& Matrix3::Translate(const Vector2& v) {
		*this = *this * Matrix3::CreateTranslation(v);

		return *this;
	}

	Matrix3& Matrix3::Scale(const Vector2& v) {
		*this = *this * Matrix3::CreateScale(v);

		return *this;
	}

	Matrix3& Matrix3::Rotate(float theta) {
		*this = *this * Matrix3::CreateRotation(theta);

		return *this;
	}

	Matrix3 Matrix3::CreateTranslation(const Vector2& v) {
		return Matrix3({
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			v.x, v.y, 1.0f,
		});
	}

	Matrix3 Matrix3::CreateScale(const Vector2& v) {
		return Matrix3({
			v.x, 0.f, 0.f,
			0.f, v.y, 0.f,
			0.f, 0.f, 1.f,
		});
	}

	Matrix3 Matrix3::CreateRotation(float theta) {
		return Matrix3({
			Math::cos(theta), Math::sin(theta), 0.0f,
			-Math::sin(theta), Math::cos(theta), 0.0f,
			0.0f, 0.0f, 1.0f,
		});
	}

	static float Det(float a, float b, float c, float d) {
		return (a * d) - (b * c);
	}

	static float Det(const Matrix3& m) {
		return
			((m[0] * m[4] * m[8]) + (m[3] * m[7] * m[2]) + (m[6] * m[1] * m[5]))
				- ((m[6] * m[4] * m[2]) + (m[0] * m[7] * m[5]) + (m[3] * m[1] * m[8]));
	}

	Matrix3 Matrix3::CreateInverse(const Matrix3& m) {
		float determinant = Det(m);

		if (determinant == 0.0)
			return Matrix3::zero;

		float oo = 1 / determinant;

		// 1/det(M) * (coef(m))^-T // in one take
		return Matrix3({
			oo * Det(m[4], m[7], m[5], m[8]), // 0
			oo * -Det(m[1], m[7], m[2], m[8]), // 1
			oo * Det(m[1], m[4], m[2], m[5]), // 2

			oo * -Det(m[3], m[6], m[5], m[8]), // 3
			oo * Det(m[0], m[6], m[2], m[8]), // 4
			oo * -Det(m[0], m[3], m[2], m[5]), // 5

			oo * Det(m[3], m[6], m[4], m[7]), // 6
			oo * -Det(m[0], m[6], m[1], m[7]), // 7
			oo * Det(m[0], m[3], m[1], m[4])  // 8
		});
	}

}


