#include "Vector.hpp"
#include "Matrix.hpp"

namespace Anggur {

const Matrix Matrix::identity({
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
});

const Matrix Matrix::zero({
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
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
        data[i] = *it;
}

const float* Matrix::ToFloatPtr() const
{
    return data;
}

float* Matrix::ToFloatPtr()
{
    return data;
}

float& Matrix::operator[](size_t index)
{
    return data[index];
}

float Matrix::operator[](size_t index) const
{
    return data[index];
}

Matrix Matrix::operator*(const Matrix& right) const
{
    Matrix result;

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

Matrix Matrix::operator+(const Matrix& right) const
{
    Matrix result;

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
    data[6] += v.x;
    data[7] += v.y;

    return *this;
}

Matrix& Matrix::Scale(const Vector& v)
{
    data[0] *= v.x;
    data[4] *= v.y;

    return *this;
}

Matrix& Matrix::Rotate(float theta)
{
    Matrix right = Matrix::CreateRotation(theta);
    Matrix result;

    result[0] = data[0] * right[0] + data[3] * right[1] + data[6] * right[2];
    result[1] = data[1] * right[0] + data[4] * right[1] + data[7] * right[2];
    result[3] = data[0] * right[3] + data[3] * right[4] + data[6] * right[5];
    result[4] = data[1] * right[3] + data[4] * right[4] + data[7] * right[5];

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
        return Matrix::zero;

    float oo = 1 / determinant;

    // 1/det(M) * (coef(m))^-T // in one take
    return Matrix({
        oo *  Det(m[4], m[7], m[5], m[8]), // 0
        oo * -Det(m[1], m[7], m[2], m[8]), // 1
        oo *  Det(m[1], m[4], m[2], m[5]), // 2

        oo * -Det(m[3], m[6], m[5], m[8]), // 3
        oo *  Det(m[0], m[6], m[2], m[8]), // 4
        oo * -Det(m[0], m[3], m[2], m[5]), // 5

        oo *  Det(m[3], m[6], m[4], m[7]), // 6
        oo * -Det(m[0], m[6], m[1], m[7]), // 7
        oo *  Det(m[0], m[3], m[1], m[4])  // 8
    });
}

}


