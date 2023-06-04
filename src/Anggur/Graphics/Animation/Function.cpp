#include "Anggur/Graphics/Animation/Function.h"
#include "Anggur/Math/Math.h"

namespace Anggur
{
	float Linear(float step)
	{
		return step;
	}

	float EaseIn(float step)
	{
		return step * step;
	}

	float EaseOut(float step)
	{
		return 1.0f - Math::Pow(1.0f - step, 2);
	}

	float EaseInOut(float step)
	{
		return step < 0.5f ? 2.0f * step * step : 1.0f - Math::Pow(-2.0f * step + 2.0f, 2) / 2.0f;
	}

	float Elastic(float step)
	{
		float c4 = (2 * Math::pi) / 3.0f;
		return step == 0.0f ? 0.0f
		: step == 1.0f ? 1.0f
		: Math::Pow(2.0f, -10.0f * step) * Math::Sin((step * 10.0f - 0.75f) * c4) + 1.0f;
	}

	float Bounce(float step)
	{
		if (step < (1.0f / 2.75f))
		{
				return 7.5625f * step * step;
		}
		else if (step < (2.0f / 2.75f))
		{
			step -= (1.5f / 2.75f);
			return 7.5625f * step * step + 0.75f;
		}
		else if (step < (2.5f / 2.75f))
		{
			step -= (2.25f / 2.75f);
			return 7.5625f * step * step + 0.9375f;
		}
		else
		{
			step -= (2.625f / 2.75f);
			return 7.5625f * step * step + 0.984375f;
		}
	}
}
