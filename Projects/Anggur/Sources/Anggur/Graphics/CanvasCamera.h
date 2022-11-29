#pragma once

#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur
{
    class CanvasCamera
    {
      public:
        void update()
        {
            if (!isDirty())
            {
                return;
            }

            clean();

            viewProjection =
                Matrix3::CreateTranslation(
                    Vector2(targetPosition.x + targetOffset.x, targetPosition.y + targetOffset.y)) *
                Matrix3::CreateRotation(viewRotation) *
                Matrix3::CreateScale(Vector2(2.0f / (viewSize.x * viewScale.x), -2.0f / (viewSize.y * viewScale.y)));

            screen = viewProjection.GetInverse();
        }

        const Matrix3& getViewProjection() const
        {
            return viewProjection;
        }

        const Matrix3& getScreen() const
        {
            return screen;
        }

        const Vector2& getTargetPosition() const
        {
            return targetPosition;
        }

        Vector2 toWorldPoint(const Vector2& screenPoint) const
        {
            Vector2 normalizedScreenPoint =
                Vector2(screenPoint.x / screenSize.x * 2 - 1, -screenPoint.y / screenSize.y * 2 + 1);
            return screen * normalizedScreenPoint;
        }

        void setViewSize(const Vector2& size)
        {
            markDirty();
            viewSize.Set(Math::Max(Math::epsilon, size.x), Math::Max(Math::epsilon, size.y));
        }

        void setViewScale(const Vector2& scale)
        {
            markDirty();
            viewScale.Set(Math::Max(Math::epsilon, scale.x), Math::Max(Math::epsilon, scale.y));
        }

        void setViewRotation(float rotation)
        {
            markDirty();
            viewRotation = Math::Mod(rotation, Math::twoPi);
        }

        void setTargetPosition(const Vector2& position)
        {
            markDirty();
            targetPosition.Set(position.x, position.y);
        }

        void setTargetOffset(const Vector2& offset)
        {
            markDirty();
        }

        void setScreenSize(const Vector2& size)
        {
            markDirty();
            screenSize.Set(size.x, size.y);
        }

        void nudgeViewScale(const Vector2& amount)
        {
            markDirty();
            setViewScale(viewScale + amount);
        }

        void nudgeViewRotation(float rotation)
        {
            markDirty();
            setViewRotation(viewRotation + rotation);
        }

        void nudgeTargetPosition(const Vector2& amount)
        {
            markDirty();
            targetPosition += amount;
        }

        bool isDirty()
        {
            return dirty;
        }

        void markDirty()
        {
            dirty = true;
        }

        void clean()
        {
            dirty = false;
        }

      private:
        bool dirty = true;

        Vector2 targetPosition;
        Vector2 targetOffset;
        float viewRotation = 0;
        Vector2 viewSize;
        Vector2 viewScale = Vector2(1.0f);
        Matrix3 viewProjection;

        Vector2 screenSize;
        Matrix3 screen;
    };
}