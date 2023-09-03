#include "Anggur/Math/Random.h"
#include "Anggur/Math.h"

namespace Anggur
{
	Random::Random() : _generator(_device())
	{
	}

	float Random::Generate()
	{
		return GetInstance()._generator();
	}

	float Random::Range(float lower, float upper)
	{
		std::uniform_real_distribution<float> distribution(lower, upper);
		return distribution(GetInstance()._generator);
	}

	float Random::GetNormal()
	{
		return Range(0.f, 1.f);
	}

	float Random::GetSigned()
	{
		return Range(-1.f, 1.f);
	}

	float Random::GetAngle()
	{
		return Range(0.f, Math::twoPi);
	}

	bool Random::GetBoolean()
	{
		return GetNormal() > 0.5;
	}

	void Random::SetSeed(float n)
	{
		GetInstance()._generator.seed(n);
	}
}
