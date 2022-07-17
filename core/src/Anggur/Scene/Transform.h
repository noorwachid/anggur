#pragma once

#include "Anggur/Math/Vector3.h"

namespace Anggur
{
	struct Transform 
	{
		Vector3 translation;
		Quaternion rotation; 
		Vector3 scale;

		Matrix4 localModel;
		Matrix4 worldModel;

		Matrix4 ToMatrix4()
		{
			return Matrix4::CreateScale(scale) * Matrix4::CreateRotation(rotation) * Matrix4::CreateTranslation(translation);
		}
	};
}