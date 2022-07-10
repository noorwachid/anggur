#include "Math.h"
#include "Vector3.h"
#include "Matrix4.h"

namespace Anggur 
{
	Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
	Vector3 Vector3::one(1.0f, 1.0f, 1.0f);

	Vector3 Vector3::unitX(1.0f, 0.0f, 0.0f);
	Vector3 Vector3::unitY(0.0f, 1.0f, 0.0f);
	Vector3 Vector3::unitZ(0.0f, 0.0f, 1.0f);

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