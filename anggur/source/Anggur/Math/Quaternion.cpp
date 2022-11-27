#include "Math.h"
#include "Quaternion.h"

namespace Anggur {    
    // Initalizers

    Quaternion::Quaternion(): x(0.0f), y(0.0f), z(0.0f), w(0.0f) {
    }

    Quaternion::Quaternion(float newX, float newY, float newZ, float newW):  x(newX), y(newY), z(newZ), w(newW) {
    }

    Quaternion::Quaternion(const Vector3& axis, float angle) {
        set(axis, angle);
    }

    // Getters

    float Quaternion::getLengthSquared() {
        return (x*x + y*y + z*z + w*w);
    }

    float Quaternion::getLength() {
        return Math::sqrt(getLengthSquared());
    }

    // Setters

    void Quaternion::set(float newX, float newY, float newZ, float newW) {
        x = newX;
        y = newY;
        z = newZ;
        w = newW;
    }

    void Quaternion::set(const Vector3& axis, float angle) {
        float scalar = Math::sin(angle / 2.0f);
        x = axis.x * scalar;
        y = axis.y * scalar;
        z = axis.z * scalar;
        w = Math::cos(angle / 2.0f);
    }

    // 1st class manipulations

    void Quaternion::conjugate() {
        x *= -1.0f;
        y *= -1.0f;
        z *= -1.0f;
    }

    // 2nd class manipulations

    static float dot(const Quaternion& a, const Quaternion& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    Quaternion Quaternion::lerp(const Quaternion& a, const Quaternion& b, float amount) {
        Quaternion newValue;

        newValue.x = Math::lerp(a.x, b.x, amount);
        newValue.y = Math::lerp(a.y, b.y, amount);
        newValue.z = Math::lerp(a.z, b.z, amount);
        newValue.w = Math::lerp(a.w, b.w, amount);

        return normalize(newValue);
    }

    Quaternion Quaternion::slerp(const Quaternion& a, const Quaternion& b, float amount) {
        float rawCosm = Quaternion::dot(a, b);
        float cosom = -rawCosm;

        if (rawCosm >= 0.0f)
            cosom = rawCosm;

        float scale0, scale1;

        if (cosom < 0.9999f) {
            const float omega = Math::acos(cosom);
            const float invSin = 1.f / Math::sin(omega);
            scale0 = Math::sin((1.f - amount) * omega) * invSin;
            scale1 = Math::sin(amount * omega) * invSin;
        } else {
            // Use linear interpolation if the quaternions
            // are collinear
            scale0 = 1.0f - amount;
            scale1 = amount;
        }

        if (rawCosm < 0.0f)
            scale1 = -scale1;

        Quaternion newValue;
        newValue.x = scale0 * a.x + scale1 * b.x;
        newValue.y = scale0 * a.y + scale1 * b.y;
        newValue.z = scale0 * a.z + scale1 * b.z;
        newValue.w = scale0 * a.w + scale1 * b.w;

        return normalize(newValue);
    }

    Quaternion Quaternion::concatenate(const Quaternion& q, const Quaternion& p) {
        Quaternion newValue;

        // Vector component is:
        // ps * qv + qs * pv + pv x qv
        Vector3 qv(q.x, q.y, q.z);
        Vector3 pv(p.x, p.y, p.z);
        Vector3 newVec = p.w * qv + q.w * pv + Vector3::cross(pv, qv);
        newValue.x = newVec.x;
        newValue.y = newVec.y;
        newValue.z = newVec.z;

        // Scalar component is:
        // ps * qs - pv . qv
        newValue.w = p.w * q.w - Vector3::dot(pv, qv);

        return newValue;
    }
}

