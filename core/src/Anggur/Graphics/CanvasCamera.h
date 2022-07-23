#pragma once

#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Matrix3.h"

namespace Anggur {
	class CanvasCamera {
    public:
        void update() {
            if (!isDirty()) {
                return;
            }

            clean();
            
            viewProjection.set(
                2.0f / (frameSize.x * frameScale.x), 0.0f, 0.0f,
                0.0f, -2.0f / (frameSize.y * frameScale.y), 0.0f,
                0.0f, 0.0f, 1.0f
            );
        }

        const Matrix3& getViewProjection() const {
            return viewProjection;
        }

        void setFrameSize(const Vector2& newFrameSize) {
            markDirty();
            frameSize.set(Math::max(1.0f, newFrameSize.x), Math::max(1.0f, newFrameSize.y));
        }

        void setFrameScale(const Vector2& scale) {
            markDirty();
            frameScale.set(scale.x, scale.y);
        }

        void setTargetPosition(const Vector2& position) {
            markDirty();
        }

        void setTargetOffset(const Vector2& offset) {
            markDirty();
        }

        bool isDirty() {
            return dirty;
        }

        void markDirty() {
            dirty = true;
        }

        void clean() {
            dirty = false;
        }

    private:
        bool dirty = true;

        Vector2 targetPosition;
        Vector2 targetOffset;
        Vector2 frameSize;
        Vector2 frameScale = Vector2(1.0f);

        Matrix3 viewProjection;
    };
}