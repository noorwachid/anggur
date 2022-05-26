#pragma once

#include <Anggur/Math/Matrix3.h>

namespace Anggur {
    Matrix3 createScreenVP(const Vector2& screenSize, float scale = 1.0f) {
        return Matrix3({
            2.0f / screenSize.x * scale, 0.0f, 0.0f,
            0.0f, -2.0f / screenSize.y * scale, 0.0f,
            -1.0f, 1.0f, 1.0f,
        });
    }

    Matrix3 createVP(float aspectRatio, float scale = 1.0f) {
        return Matrix3({
            1.0f * scale, 0.0f, 0.0f,
            0.0f, aspectRatio * scale, 0.0f,
            0.0f, 0.0f, 1.0f,
        });
    }
}