#pragma once

#include <initializer_list>
#include "Math.h"

namespace Anggur {
	class Matrix3 {
	public:
		static const Matrix3 identity;
		static const Matrix3 zero;

		Matrix3();
		Matrix3(const std::initializer_list<float>& list);
		Matrix3(const Vector2& translation, float rotation = 0.0f, const Vector2& scale = 1.0f);

		void Set(const std::initializer_list<float>& list);
		const float* GetData() const;
		float* GetData();

		float& operator[](size_t index);
		float operator[](size_t index) const;

		Matrix3 operator*(const Matrix3& right) const;
		Matrix3 operator+(const Matrix3& right) const;

		Matrix3& operator*=(const Matrix3& right);
		Matrix3& operator+=(const Matrix3& right);

		Matrix3& Translate(const Vector2& v);
		Matrix3& Scale(const Vector2& v);
		Matrix3& Rotate(float theta);

		static Matrix3 CreateTranslation(const Vector2& v);
		static Matrix3 CreateScale(const Vector2& v);
		static Matrix3 CreateRotation(float theta);
		static Matrix3 CreateInverse(const Matrix3& m);

	private:
		float _data[9];
	};
}
