#include <Anggur/Math/Common.h>
#include <Anggur/Math/Vector3.h>

namespace Anggur {
	Vector3 Vector3::lerp(const Vector3& a, const Vector3& b, float amount) {
		Vector3 v;
		v.x = Math::lerp(a.x, b.x, amount);
		v.y = Math::lerp(a.y, b.y, amount);
		v.z = Math::lerp(a.z, b.z, amount);

		return v;
	}
}