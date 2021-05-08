#include "Vector.hpp"
#include "Matrix.hpp"

namespace Anggur {

const Matrix Matrix::Identity({
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
});

const Matrix Matrix::Zero({
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
});

Matrix::Matrix()
{
    *this = Matrix::Identity;
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
    Matrix right = Matrix::CreateRotation(theta);
    Matrix result;

    result[0] = mData[0] * right[0] + mData[3] * right[1] + mData[6] * right[2];
    result[1] = mData[1] * right[0] + mData[4] * right[1] + mData[7] * right[2];
    result[3] = mData[0] * right[3] + mData[3] * right[4] + mData[6] * right[5];
    result[4] = mData[1] * right[3] + mData[4] * right[4] + mData[7] * right[5];

    *this = result;

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

static float Det(float a, float b, float c, float d)
{
    return (a * d) - (b * c);
}

static float Det(const Matrix& m)
{
    return
        ((m[0] * m[4] * m[8]) + (m[3] * m[7] * m[2]) + (m[6] * m[1] * m[5]))
       -((m[6] * m[4] * m[2]) + (m[0] * m[7] * m[5]) + (m[3] * m[1] * m[8]));
}

Matrix Matrix::CreateInverse(const Matrix& m)
{
    float determinant = Det(m);

    if (determinant == 0.0)
        return Matrix::Zero;

    float oo = 1 / determinant;

    // 1/det(M) * (coef(m))^-T // in one take
    return Matrix({
        oo *  Det(m[4], m[7], m[5], m[8]), // 0
        oo * -Det(m[3], m[6], m[5], m[8]), // 3
        oo *  Det(m[3], m[6], m[4], m[7]), // 6

        oo * -Det(m[1], m[7], m[2], m[8]), // 1
        oo *  Det(m[0], m[6], m[2], m[8]), // 4
        oo * -Det(m[0], m[6], m[1], m[7]), // 7

        oo *  Det(m[1], m[4], m[2], m[5]), // 2
        oo * -Det(m[0], m[3], m[2], m[5]), // 5
        oo *  Det(m[0], m[3], m[1], m[4])  // 8
    });
}

}


