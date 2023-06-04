#pragma once

namespace Anggur
{
	using AnimationFunction = float(*)(float step);

	float Linear(float step);

	float EaseIn(float step);

	float EaseOut(float step);

	float EaseInOut(float step);

	float Elastic(float step);

	float Bounce(float step);
}


