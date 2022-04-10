#include "Anggur/Core/Scene/Transform.h"
#include "Anggur/Math/Matrix3.h"

namespace Anggur {
	Transform::Transform(const Vector2& translation, float rotation, const Vector2& scale):
		translation(translation),
		rotation(rotation),
		scale(scale) {
	}

	Matrix3 Transform::toMatrix3() {
		Matrix3 matrix = Matrix3::createScale(scale);
		matrix.rotate(rotation);
		matrix.translate(translation);

		return matrix;
	}
}
