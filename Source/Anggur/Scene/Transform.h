#pragma once

#include "Anggur/Math/Matrix4.h"
#include "Anggur/Math/Quaternion.h"
#include "Anggur/Math/Vector3.h"

namespace Anggur
{
	struct Transform
	{
		Vector3 position;
		Quaternion rotation;
		Vector3 scale = Vector3::one;

		Matrix4 ToMatrix()
		{
			return Matrix4::CreateScale(scale) * Matrix4::CreateRotation(rotation) *
				   Matrix4::CreateTranslation(position);
		}
	};
}
