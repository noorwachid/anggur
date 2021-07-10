#include "Vector.hpp"
#include "Matrix.hpp"

namespace Anggur {

Vector Vector::Zero(0, 0);
Vector Vector::One(1, 1);

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
    return &X;
}

float* Vector::ToFloatPtr()
{
    return &X;
}

void Vector::Set(float v)
{
    X = v;
    Y = v;
}

void Vector::Set(float x, float y)
{
    X = x;
    Y = y;
}

void Vector::SetPolar(float angle, float length)
{
    X = Math::Cos(angle) * length;
    Y = Math::Sin(angle) * length;
}

Vector Vector::operator+(const Vector& v) const
{
    return Vector(X + v.X, Y + v.Y);
}

Vector Vector::operator-(const Vector& v) const
{
    return Vector(X - v.X, Y - v.Y);
}

Vector Vector::operator*(const Vector& v) const
{
    return Vector(X * v.X, Y * v.Y);
}

Vector Vector::operator+(float scalar) const
{
    return Vector(X + scalar, Y + scalar);
}

Vector Vector::operator-(float scalar) const
{
    return Vector(X - scalar, Y - scalar);
}

Vector Vector::operator*(float scalar) const
{
    return Vector(X * scalar, Y * scalar);
}

Vector Vector::operator/(float scalar) const
{
        return Vector(X / scalar, Y / scalar);
}

Vector Vector::operator*(const Matrix& m) const
{
    return Vector(
        m[0] * X + m[3] * Y + m[6],
        m[1] * X + m[4] * Y + m[7]
    );
}

Vector Vector::operator-() const
{
    return Vector(-X, -Y);
}

Vector& Vector::operator+=(float scalar)
{
    X += scalar;
    Y += scalar;
    return *this;
}

Vector& Vector::operator-=(float scalar)
{
    X -= scalar;
    Y -= scalar;
    return *this;
}

Vector& Vector::operator*=(float scalar)
{
    X *= scalar;
    Y *= scalar;
    return *this;
}

Vector& Vector::operator/=(float scalar)
{
    X /= scalar;
    Y /= scalar;
    return *this;
}

Vector& Vector::operator+=(const Vector& other)
{
    X += other.X;
    Y += other.Y;
    return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
    X -= other.X;
    Y -= other.Y;
    return *this;
}

Vector& Vector::operator*=(const Matrix& m)
{
    *this = *this * m;
    return *this;
}

float Vector::GetLengthSq() const
{
    return (X*X + Y*Y);
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
    return Math::Atan(Y, X);
}

Vector& Vector::SetAngle(float theta)
{
    SetPolar(theta, GetLength());
    return *this;
}

Vector Vector::GetPerpen() const
{
    return Vector(-Y, X);
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
    X /= t;
    Y /= t;
    return *this;
}

float Vector::Dot(const Vector& a, const Vector& b)
{
    return a.X * b.X + a.Y * b.Y;
}

float Vector::Cross(const Vector& a, const Vector& b)
{
    return a.X * b.Y - a.Y * b.X;
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
    float pa =  a.X - b.X;
    float pb =  a.Y - b.Y;

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
    return (c.X * c.X) + (c.Y * c.Y);
}

Vector Vector::CreatePolar(float angle, float length)
{
    Vector temp;
    temp.SetPolar(angle, length);
    return temp;
}

}
