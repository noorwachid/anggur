#pragma once

#include "Anggur/Math/Vector3.h";

namespace Anggur
{
    struct Transform
    {
        Vector3 translation;
        Vector3 rotation;
        Vector3 scale;
    };
}