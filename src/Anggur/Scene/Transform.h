#pragma once 

#include "Anggur/Math/Matrix4.h"
#include "Anggur/Math/Quaternion.h"
#include "Anggur/Math/Vector3.h"

namespace Anggur 
{
    struct Transform 
    {
        Vector3 position = Vector3::zero;
        Vector3 rotation = Vector3::zero;
        Vector3 scale = Vector3::one;

        Matrix4 ToMatrix()
        {
            return Matrix4::CreateScale(scale) * Matrix4::CreateTranslation(position);
        }
    };
}
