#pragma once

namespace Anggur {
	using AnimationFunction = float (*)(float step);

	float linear(float step);

	float easeIn(float step);

	float easeOut(float step);

	float easeInOut(float step);

	float elastic(float step);

	float bounce(float step);
}
