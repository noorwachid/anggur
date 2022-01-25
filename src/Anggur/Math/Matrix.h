#pragma once

#include <initializer_list>
#include "Math.h"

namespace Anggur
{
	class Matrix
	{
	public:
		static const Matrix identity;
		static const Matrix zero;

		Matrix();
		Matrix(const std::initializer_list<float>& list);
		Matrix(const Vector& translation, float rotation = 0.0f, const Vector& scale = 1.0f);

		void Set(const std::initializer_list<float>& list);
		const float* GetData() const;
		float* GetData();

		float& operator[](size_t index);
		float operator[](size_t index) const;

		Matrix operator*(const Matrix& right) const;
		Matrix operator+(const Matrix& right) const;

		Matrix& operator*=(const Matrix& right);
		Matrix& operator+=(const Matrix& right);

		Matrix& Translate(const Vector& v);
		Matrix& Scale(const Vector& v);
		Matrix& Rotate(float theta);

		static Matrix CreateTranslation(const Vector& v);
		static Matrix CreateScale(const Vector& v);
		static Matrix CreateRotation(float theta);
		static Matrix CreateInverse(const Matrix& m);

	private:
		float _data[9];
	};
}
