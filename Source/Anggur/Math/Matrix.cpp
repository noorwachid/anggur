#include "Vector.hpp"
#include "Matrix.hpp"

namespace Anggur {

const Matrix Matrix::identity({
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
});

Matrix::Matrix()
{
    *this = Matrix::identity;
}

Matrix::Matrix(const std::initializer_list<float>& list)
{
    Set(list);
}

void Matrix::Set(const std::initializer_list<float>& list)
{
    size_t i = 0;
    for (auto it = list.begin(); it != list.end(); ++it, ++i)
        mData[i] = *it;
}

const float* Matrix::ToFloatPtr() const
{
    return mData;
}

float* Matrix::ToFloatPtr()
{
    return mData;
}

float& Matrix::operator[](size_t index)
{
    return mData[index];
}

float Matrix::operator[](size_t index) const
{
    return mData[index];
}

Matrix Matrix::operator*(const Matrix& right) const
{
    Matrix result;

    result[0] = mData[0] * right[0] + mData[3] * right[1] + mData[6] * right[2];
    result[1] = mData[1] * right[0] + mData[4] * right[1] + mData[7] * right[2];
    result[2] = mData[2] * right[0] + mData[5] * right[1] + mData[8] * right[2];

    result[3] = mData[0] * right[3] + mData[3] * right[4] + mData[6] * right[5];
    result[4] = mData[1] * right[3] + mData[4] * right[4] + mData[7] * right[5];
    result[5] = mData[2] * right[3] + mData[5] * right[4] + mData[8] * right[5];

    result[6] = mData[0] * right[6] + mData[3] * right[7] + mData[6] * right[8];
    result[7] = mData[1] * right[6] + mData[4] * right[7] + mData[7] * right[8];
    result[8] = mData[2] * right[6] + mData[5] * right[7] + mData[8] * right[8];

    return result;
}

Matrix Matrix::operator+(const Matrix& right) const
{
    Matrix result;

    result[0] = mData[0] + right[0];
    result[1] = mData[1] + right[1];
    result[2] = mData[2] + right[2];

    result[3] = mData[3] + right[3];
    result[4] = mData[4] + right[4];
    result[5] = mData[5] + right[5];

    result[6] = mData[6] + right[6];
    result[7] = mData[7] + right[7];
    result[8] = mData[8] + right[8];

    return result;
}

Matrix& Matrix::operator*=(const Matrix& right)
{
    *this = *this * right;
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& right)
{
    *this = *this + right;
    return *this;
}

Matrix& Matrix::Translate(const Vector& v)
{
    mData[6] += v.x;
    mData[7] += v.y;

    return *this;
}

Matrix& Matrix::Scale(const Vector& v)
{
    mData[0] *= v.x;
    mData[4] *= v.y;

    return *this;
}

Matrix& Matrix::Rotate(float theta)
{
    mData[0] *= Math::Cos(theta);
    mData[1] *= Math::Sin(theta);
    mData[3] *= -Math::Sin(theta);
    mData[4] *= Math::Cos(theta);

    return *this;
}

Matrix Matrix::CreateTranslation(const Vector& v)
{
    return Matrix({
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        v.x, v.y, 1.0f,
    });
}

Matrix Matrix::CreateScale(const Vector& v)
{
    return Matrix({
        v.x, 0.f, 0.f,
        0.f, v.y, 0.f,
        0.f, 0.f, 1.f,
    });
}

Matrix Matrix::CreateRotation(float theta)
{
    return Matrix({
        Math::Cos(theta), Math::Sin(theta), 0.0f,
        -Math::Sin(theta), Math::Cos(theta), 0.0f,
        0.0f, 0.0f, 1.0f,
    });
}

}
