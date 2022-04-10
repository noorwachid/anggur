#include "Anggur/Core/Scene/Transform.h"
#include "Anggur/Math/Matrix3.h"

namespace Anggur {
	Matrix3 Transform::toMatrix3() {
		Matrix3 matrix = Matrix3::createScale(scale);
		matrix.rotate(rotation);
		matrix.translate(translation);

		return matrix;
	}
}
