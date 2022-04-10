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
		set(list);
	}

	Matrix3::Matrix3(const Vector2& translation, float rotation, const Vector2& scale) {
		*this = Matrix3::identity;
		this->scale(scale);
		this->rotate(rotation);
		this->translate(translation);
	}

	void Matrix3::set(const std::initializer_list<float>& list) {
		size_t i = 0;
		for (auto it = list.begin(); it != list.end(); ++it, ++i)
			data[i] = *it;
	}

	const float* Matrix3::toPointer() const {
		return data;
	}

	float* Matrix3::toPointer() {
		return data;
	}

	float& Matrix3::operator[](size_t index) {
		return data[index];
	}

	float Matrix3::operator[](size_t index) const {
		return data[index];
	}

	Matrix3 Matrix3::operator*(const Matrix3& right) const {
		Matrix3 result;

		result[0] = data[0] * right[0] + data[3] * right[1] + data[6] * right[2];
		result[1] = data[1] * right[0] + data[4] * right[1] + data[7] * right[2];
		result[2] = data[2] * right[0] + data[5] * right[1] + data[8] * right[2];

		result[3] = data[0] * right[3] + data[3] * right[4] + data[6] * right[5];
		result[4] = data[1] * right[3] + data[4] * right[4] + data[7] * right[5];
		result[5] = data[2] * right[3] + data[5] * right[4] + data[8] * right[5];

		result[6] = data[0] * right[6] + data[3] * right[7] + data[6] * right[8];
		result[7] = data[1] * right[6] + data[4] * right[7] + data[7] * right[8];
		result[8] = data[2] * right[6] + data[5] * right[7] + data[8] * right[8];

		return result;
	}

	Matrix3 Matrix3::operator+(const Matrix3& right) const {
		Matrix3 result;

		result[0] = data[0] + right[0];
		result[1] = data[1] + right[1];
		result[2] = data[2] + right[2];

		result[3] = data[3] + right[3];
		result[4] = data[4] + right[4];
		result[5] = data[5] + right[5];

		result[6] = data[6] + right[6];
		result[7] = data[7] + right[7];
		result[8] = data[8] + right[8];

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

	Matrix3& Matrix3::translate(const Vector2& translation) {
		*this = *this * Matrix3::createTranslation(translation);

		return *this;
	}

	Matrix3& Matrix3::scale(const Vector2& scale) {
		*this = *this * Matrix3::createScale(scale);

		return *this;
	}

	Matrix3& Matrix3::rotate(float rotation) {
		*this = *this * Matrix3::createRotation(rotation);

		return *this;
	}

	Matrix3 Matrix3::createTranslation(const Vector2& translation) {
		return Matrix3({
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			translation.x, translation.y, 1.0f,
		});
	}

	Matrix3 Matrix3::createScale(const Vector2& scale) {
		return Matrix3({
			scale.x, 0.f, 0.f,
			0.f, scale.y, 0.f,
			0.f, 0.f, 1.f,
		});
	}

	Matrix3 Matrix3::createRotation(float rotation) {
		return Matrix3({
			Math::cos(rotation), Math::sin(rotation), 0.0f,
			-Math::sin(rotation), Math::cos(rotation), 0.0f,
			0.0f, 0.0f, 1.0f,
		});
	}

	static float det(float a, float b, float c, float d) {
		return (a * d) - (b * c);
	}

	static float det(const Matrix3& m) {
		return
			((m[0] * m[4] * m[8]) + (m[3] * m[7] * m[2]) + (m[6] * m[1] * m[5]))
				- ((m[6] * m[4] * m[2]) + (m[0] * m[7] * m[5]) + (m[3] * m[1] * m[8]));
	}

	Matrix3 Matrix3::createInverse(const Matrix3& original) {
		float determinant = det(original);

		if (determinant == 0.0)
			return Matrix3::zero;

		float oo = 1 / determinant;

		// 1/det(M) * (coef(original))^-T // in one take
		return Matrix3({
			oo * det(original[4], original[7], original[5], original[8]), // 0
			oo * -det(original[1], original[7], original[2], original[8]), // 1
			oo * det(original[1], original[4], original[2], original[5]), // 2

			oo * -det(original[3], original[6], original[5], original[8]), // 3
			oo * det(original[0], original[6], original[2], original[8]), // 4
			oo * -det(original[0], original[3], original[2], original[5]), // 5

			oo * det(original[3], original[6], original[4], original[7]), // 6
			oo * -det(original[0], original[6], original[1], original[7]), // 7
			oo * det(original[0], original[3], original[1], original[4])  // 8
		});
	}

}


