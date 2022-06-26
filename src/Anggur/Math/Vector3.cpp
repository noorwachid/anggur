#include "Math.h"
#include "Vector3.h"

namespace Anggur 
{
	void Vector3::Set(float newX, float newY, float newZ)
	{
		x = newX;
		y = newY;
		z = newZ;
	}

	Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float amount) 
	{
		return Vector3(
			Math::Lerp(a.x, b.x, amount),
			Math::Lerp(a.y, b.y, amount),
			Math::Lerp(a.z, b.z, amount)
		);
	}
}