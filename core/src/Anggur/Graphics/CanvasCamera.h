#pragma once

#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Matrix3.h"

namespace Anggur 
{
	class CanvasCamera 
    {
    public:
        void Update() 
        {
            if (!IsDirty()) 
            {
                return;
            }

            Clean();
            
            viewProjection = 
                Matrix3::CreateRotation(frameRotation)
                *
                Matrix3(
                    2.0f / (frameSize.x * frameScale.x), 0.0f, 0.0f,
                    0.0f, -2.0f / (frameSize.y * frameScale.y), 0.0f,
                    0.0f, 0.0f, 1.0f
                ) 
                *
                Matrix3(
                    1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    // 0.0f, 0.0f, 1.0f
                    -targetPosition.x + targetOffset.x, targetPosition.y + targetOffset.y, 1.0f
                )
            ;
        }

        const Matrix3& GetViewProjection() const 
        {
            return viewProjection;
        }

        void SetFrameSize(const Vector2& size) 
        {
            MarkDirty();
            frameSize.Set(Math::Max(Math::epsilon, size.x), Math::Max(Math::epsilon, size.y));
        }

        void SetFrameScale(const Vector2& scale) 
        {
            MarkDirty();
            frameScale.Set(Math::Max(Math::epsilon, scale.x), Math::Max(Math::epsilon, scale.y));
        }

        void SetFrameRotation(float rotation)
        {
            MarkDirty();
            frameRotation = Math::Mod(rotation, Math::twoPi);
        }

        void SetTargetPosition(const Vector2& position) 
        {
            MarkDirty();
            targetPosition.Set(position.x, position.y);
        }

        void SetTargetOffset(const Vector2& offset) 
        {
            MarkDirty();
        }

        void NudgeFrameScale(const Vector2& amount)
        {
            MarkDirty();
            SetFrameScale(frameScale + amount);
        }

        void NudgeFrameRotation(float rotation)
        {
            MarkDirty();
            SetFrameRotation(frameRotation + rotation);
        }

        void NudgeTargetPosition(const Vector2& amount)
        {
            MarkDirty();
            targetPosition += amount;
        }

        bool IsDirty() 
        {
            return dirty;
        }

        void MarkDirty() 
        {
            dirty = true;
        }

        void Clean() 
        {
            dirty = false;
        }

    private:
        bool dirty = true;

        Vector2 targetPosition;
        Vector2 targetOffset;
        Vector2 frameSize;
        Vector2 frameScale = Vector2(1.0f);
        float frameRotation = 0;

        Matrix3 viewProjection;
    };
}