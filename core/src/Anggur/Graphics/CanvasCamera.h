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
                Matrix3::CreateTranslation(Vector2(-targetPosition.x + targetOffset.x, targetPosition.y + targetOffset.y)) *
                Matrix3::CreateRotation(frameRotation) *
                Matrix3::CreateScale(Vector2(2.0f / (frameSize.x * frameScale.x), -2.0f / (frameSize.y * frameScale.y)))
            ;

            screen = viewProjection.GetInverse();
        }

        const Matrix3& GetViewProjection() const 
        {
            return viewProjection;
        }

        const Matrix3& GetScreen() const
        {
            return screen;
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
        Matrix3 screen;
    };
}