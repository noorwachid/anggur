#pragma once

#include "Anggur/Math/Vector3.h"

namespace Anggur
{
    struct Transform
    {
        Vector3 translation;
        Vector3 rotation;
        Vector3 scale;

        Transform(const Vector3& newTranslation = Vector3::zero, const Vector3& newRotation = Vector3::zero, const Vector3& newScale = Vector3::one):
            translation(newTranslation), rotation(newRotation), scale(newScale)
        {
        }

        Matrix4 ToMatrix4() const
        {
            return Matrix4::CreateScale(scale) * Matrix4::CreateRotation(rotation) * Matrix4::CreateTranslation(translation);
        }
    };
}