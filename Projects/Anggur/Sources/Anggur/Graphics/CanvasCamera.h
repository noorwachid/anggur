#pragma once

#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"

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
				Matrix3::CreateTranslation(
					Vector2(targetPosition.x + targetOffset.x, targetPosition.y + targetOffset.y)) *
				Matrix3::CreateRotation(viewRotation) *
				Matrix3::CreateScale(Vector2(2.0f / (viewSize.x * viewScale.x), -2.0f / (viewSize.y * viewScale.y)));

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

		const Vector2& GetTargetPosition() const
		{
			return targetPosition;
		}

		Vector2 ToWorldPoint(const Vector2& screenPoint) const
		{
			Vector2 normalizedScreenPoint =
				Vector2(screenPoint.x / screenSize.x * 2 - 1, -screenPoint.y / screenSize.y * 2 + 1);
			return screen * normalizedScreenPoint;
		}

		void SetViewSize(const Vector2& size)
		{
			MarkDirty();
			viewSize.Set(Math::Max(Math::epsilon, size.x), Math::Max(Math::epsilon, size.y));
		}

		void SetViewScale(const Vector2& scale)
		{
			MarkDirty();
			viewScale.Set(Math::Max(Math::epsilon, scale.x), Math::Max(Math::epsilon, scale.y));
		}

		void SetViewRotation(float rotation)
		{
			MarkDirty();
			viewRotation = Math::Mod(rotation, Math::twoPi);
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

		void SetScreenSize(const Vector2& size)
		{
			MarkDirty();
			screenSize.Set(size.x, size.y);
		}

		void NudgeViewScale(const Vector2& amount)
		{
			MarkDirty();
			SetViewScale(viewScale + amount);
		}

		void NudgeViewRotation(float rotation)
		{
			MarkDirty();
			SetViewRotation(viewRotation + rotation);
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
		float viewRotation = 0;
		Vector2 viewSize;
		Vector2 viewScale = Vector2(1.0f);
		Matrix3 viewProjection;

		Vector2 screenSize;
		Matrix3 screen;
	};
}