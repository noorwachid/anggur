#pragma once

#include <cinttypes>

namespace Anggur {
	struct Vector4 {
		float x;
		float y;
		float z;
		float w;

		Vector4();
		Vector4(float x, float y, float z, float w = 1.0f);

		void set(float x, float y, float z, float w = 1.0f);

		const float* toPointer() const;

		float* toPointer();

		static Vector4 createHex(uint64_t hex);

		static Vector4 lerp(const Vector4& a, const Vector4& b, float amount);

		static const Vector4 transparent;

		static const Vector4 white;
		static const Vector4 smoke;
		static const Vector4 gray;
		static const Vector4 black;
		static const Vector4 charcoal;

		static const Vector4 red;
		static const Vector4 crimson;
		static const Vector4 salmon;
		static const Vector4 orange;
		static const Vector4 yellow;
		static const Vector4 chartreuse;
		static const Vector4 lime;
		static const Vector4 olive;
		static const Vector4 green;
		static const Vector4 seafoam;
		static const Vector4 aquamarine;
		static const Vector4 cyan;
		static const Vector4 sky;
		static const Vector4 blue;
		static const Vector4 violet;
		static const Vector4 purple;
		static const Vector4 grape;
		static const Vector4 burgundy;
		static const Vector4 fuchsia;
		static const Vector4 pink;

		static const Vector4 beige;
		static const Vector4 ivory;
		static const Vector4 brown;
		static const Vector4 chocolate;
		static const Vector4 sandy;
	};
}