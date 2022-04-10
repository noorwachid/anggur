#pragma once

#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur {
	enum class DrawMode {
		Point = 0x1B00,
		Line = 0x1B01,
		Fill = 0x1B02,
	};

	struct Camera {
	public:
		Camera() {
			origin = Vector2::zero;
			offset = Vector2::zero;
			viewport = Vector2::one;
			distance = 1.0;
			rotation = 0.0;
		}

		Matrix3 toMatrix() const {
			float distanceY = -distance * (viewport.x / viewport.y);
			Matrix3 matrix({
				distance, 0, 0,
				0, distanceY, 0,
				(-origin.x + offset.x) * distance, (-origin.y + offset.y) * distanceY, 1,
			});
			//        matrix.rotate(rotation);
			return matrix;
		}

		Vector2 toWorldPoint(const Vector2& screenPoint) {
			Vector2 result(
				(2.0 * screenPoint.x) / viewport.x - 1.0,
				-(2.0 * screenPoint.y) / viewport.y + 1.0);

			return result * Matrix3::createInverse(toMatrix());
		}

		Vector2 toScreenPoint(const Vector2& worldPoint) {
			Vector2 result(
				-worldPoint.x / viewport.x,
				worldPoint.y / viewport.y);
			return result * Matrix3::createInverse(toMatrix());
		}

		void setRatio(float ratio) {
			viewport.y = viewport.x * ratio;
		}

		float getRatio() {
			return viewport.x / viewport.y;
		}

		float distance;
		float rotation;

		Vector2 origin;
		Vector2 offset;
		Vector2 viewport;
	};

	struct TextOption {
		float size;
		float lineHeight;
		float letterSpace;
		float wordSpace;
		float isBreakable;
		std::string ellipsis;

		TextOption(float size = 1,
			float lineHeight = 1.3,
			float wordSpace = 0.2,
			float letterSpace = 0.05,
			bool isBreakable = false,
			const std::string& ellipsis = "...")
			:
			size(size),
			lineHeight(lineHeight),
			wordSpace(wordSpace),
			letterSpace(letterSpace),
			isBreakable(isBreakable),
			ellipsis(ellipsis) {
		}
	};

	struct CodepointContainer {
		Glyph glyph;
		Vector2 offset;
		Vector2 area;
	};
}
