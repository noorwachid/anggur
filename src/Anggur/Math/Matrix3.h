#pragma once

#include <initializer_list>
#include <Anggur/Math/Math.h>

namespace Anggur {
	class Matrix3 {
	public:
		static const Matrix3 identity;
		static const Matrix3 zero;

		Matrix3();
		Matrix3(const std::initializer_list<float>& list);
		Matrix3(const Vector2& translation, float rotation = 0.0f, const Vector2& scale = 1.0f);

		void set(const std::initializer_list<float>& list);
		const float* toPointer() const;
		float* toPointer();

		float& operator[](size_t index);
		float operator[](size_t index) const;

		Matrix3 operator*(const Matrix3& right) const;
		Matrix3 operator+(const Matrix3& right) const;

		Matrix3& operator*=(const Matrix3& right);
		Matrix3& operator+=(const Matrix3& right);

		Matrix3& translate(const Vector2& translation);
		Matrix3& rotate(float rotation);
		Matrix3& scale(const Vector2& scale);

		static Matrix3 createTranslation(const Vector2& translation);
		static Matrix3 createRotation(float rotation);
		static Matrix3 createScale(const Vector2& scale);
		static Matrix3 createInverse(const Matrix3& original);

	private:
		float data[9];
	};
}
