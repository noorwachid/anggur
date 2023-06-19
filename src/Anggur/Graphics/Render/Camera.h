#pragma once

#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur {
	class CanvasCamera {
	public:
		void update() {
			if (!isDirty()) {
				return;
			}

			clean();

			viewProjection =
				Matrix3::createTranslation(Vector2(targetPosition.x + targetOffset.x, targetPosition.y + targetOffset.y)
				) *
				Matrix3::createRotation(viewRotation) *
				Matrix3::createScale(Vector2(2.0f / (viewSize.x * viewScale.x), -2.0f / (viewSize.y * viewScale.y)));

			screen = viewProjection.getInverse();
		}

		const Matrix3& getViewProjection() const {
			return viewProjection;
		}

		const Matrix3& getScreen() const {
			return screen;
		}

		const Vector2& getTargetPosition() const {
			return targetPosition;
		}

		Vector2 toWorldPoint(const Vector2& screenPoint) const {
			Vector2 normalizedScreenPoint =
				Vector2(screenPoint.x / screenSize.x * 2 - 1, -screenPoint.y / screenSize.y * 2 + 1);
			return screen * normalizedScreenPoint;
		}

		void setViewSize(const Vector2& size) {
			markDirty();
			viewSize.set(Math::max(Math::epsilon, size.x), Math::max(Math::epsilon, size.y));
		}

		void setViewScale(const Vector2& scale) {
			markDirty();
			viewScale.set(Math::max(Math::epsilon, scale.x), Math::max(Math::epsilon, scale.y));
		}

		void setViewRotation(float rotation) {
			markDirty();
			viewRotation = Math::mod(rotation, Math::twoPi);
		}

		void setTargetPosition(const Vector2& position) {
			markDirty();
			targetPosition.set(position.x, position.y);
		}

		void setTargetOffset(const Vector2& offset) {
			markDirty();
		}

		void setScreenSize(const Vector2& size) {
			markDirty();
			screenSize.set(size.x, size.y);
		}

		void nudgeViewScale(const Vector2& amount) {
			markDirty();
			setViewScale(viewScale + amount);
		}

		void nudgeViewRotation(float rotation) {
			markDirty();
			setViewRotation(viewRotation + rotation);
		}

		void nudgeTargetPosition(const Vector2& amount) {
			markDirty();
			targetPosition += amount;
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
		float viewRotation = 0;
		Vector2 viewSize;
		Vector2 viewScale = Vector2(1.0f);
		Matrix3 viewProjection;

		Vector2 screenSize;
		Matrix3 screen;
	};
}
