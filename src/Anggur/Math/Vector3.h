#pragma once



namespace Anggur 
{
	struct Matrix4;

	struct Vector3 
	{
		float x;
		float y;
		float z;

		Vector3(): x(0.0f), y(0.0f), z(0.0f) 
		{
		}

		Vector3(float x, float y, float z = 1.0f): x(x), y(y), z(z) 
		{
		}

		void Set(float newX, float newY, float newZ);

		static Vector3 Lerp(const Vector3& a, const Vector3& b, float amount);

		static Vector3 zero;
		static Vector3 one;
	};
}