#include "Vector.h"
#include "Matrix.h"

namespace Anggur {

Vector Vector::zero(0, 0);
Vector Vector::one(1, 1);

Vector::Vector()
{
    Set(0.f);
}

Vector::Vector(float v)
{
    Set(v);
}

Vector::Vector(float x, float y)
{
    Set(x, y);
}

const float* Vector::ToFloatPtr() const
{
    return &x;
}

float* Vector::ToFloatPtr()
{
    return &x;
}

void Vector::Set(float v)
{
    x = v;
    y = v;
}

void Vector::Set(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Vector::SetPolar(float angle, float length)
{
    x = Math::Cos(angle) * length;
    y = Math::Sin(angle) * length;
}

Vector Vector::operator+(const Vector& v) const
{
    return Vector(x + v.x, y + v.y);
}

Vector Vector::operator-(const Vector& v) const
{
    return Vector(x - v.x, y - v.y);
}

Vector Vector::operator*(const Vector& v) const
{
    return Vector(x * v.x, y * v.y);
}

Vector Vector::operator+(float scalar) const
{
    return Vector(x + scalar, y + scalar);
}

Vector Vector::operator-(float scalar) const
{
    return Vector(x - scalar, y - scalar);
}

Vector Vector::operator*(float scalar) const
{
    return Vector(x * scalar, y * scalar);
}

Vector Vector::operator/(float scalar) const
{
        return Vector(x / scalar, y / scalar);
}

Vector Vector::operator*(const Matrix& m) const
{
    return Vector(
        m[0] * x + m[3] * y + m[6],
        m[1] * x + m[4] * y + m[7]
    );
}

Vector Vector::operator-() const
{
    return Vector(-x, -y);
}

Vector& Vector::operator+=(float scalar)
{
    x += scalar;
    y += scalar;
    return *this;
}

Vector& Vector::operator-=(float scalar)
{
    x -= scalar;
    y -= scalar;
    return *this;
}

Vector& Vector::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector& Vector::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

Vector& Vector::operator+=(const Vector& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector& Vector::operator*=(const Matrix& m)
{
    *this = *this * m;
    return *this;
}

float Vector::GetLengthSq() const
{
    return (x*x + y*y);
}

float Vector::GetLength() const
{
    return Math::Sqrt(GetLengthSq());
}

Vector& Vector::SetLength(float x)
{
    Normalize();
    *this *= x;
    return *this;
}

float Vector::GetAngle() const
{
    return Math::Atan(y, x);
}

Vector& Vector::SetAngle(float theta)
{
    SetPolar(theta, GetLength());
    return *this;
}

Vector Vector::GetPerpen() const
{
    return Vector(-y, x);
}

Vector& Vector::SetLengthLimit(float x)
{
    if (GetLengthSq() > (x * x))
        return SetLength(x);
    return *this;
}

Vector& Vector::Normalize()
{
    float t = GetLength();
    if (t == 0) return *this;
    x /= t;
    y /= t;
    return *this;
}

float Vector::Dot(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y;
}

float Vector::Cross(const Vector& a, const Vector& b)
{
    return a.x * b.y - a.y * b.x;
}

Vector Vector::Normalize(const Vector& vec)
{
    Vector temp = vec;
    temp.Normalize();
    return temp;
}

Vector Vector::Lerp(const Vector& a, const Vector& b, float amount)
{
    return Vector(a + (b - a) * amount);
}

float Vector::GetLength(const Vector& a, const Vector& b)
{
    return Math::Sqrt(GetLengthSq(a, b));
}

float Vector::GetLengthSq(const Vector& a, const Vector& b)
{
    float pa =  a.x - b.x;
    float pb =  a.y - b.y;

    return pa * pa + pb * pb;
}

float Vector::GetAngle(const Vector& a, const Vector& b)
{
    return Math::Acos(Vector::Dot(a, b));
}

float Vector::GetDist(const Vector& a, const Vector& b)
{
    return Math::Sqrt(GetDistSq(a, b));
}

float Vector::GetDistSq(const Vector& a, const Vector& b)
{
    Vector c = a - b;
    return (c.x * c.x) + (c.y * c.y);
}

Vector Vector::CreatePolar(float angle, float length)
{
    Vector temp;
    temp.SetPolar(angle, length);
    return temp;
}

}
