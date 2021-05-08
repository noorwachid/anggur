#pragma once

#include <initializer_list>
#include "Math.hpp"

namespace Anggur {

class Matrix
{
public:
    static const Matrix Identity;
    static const Matrix Zero;

    Matrix();
    Matrix(const std::initializer_list<float>& list);

    void Set(const std::initializer_list<float>& list);
    const float* ToFloatPtr() const;
    float* ToFloatPtr();

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
    float mData[9];

};

}
