#include "Anggur/Math/Random.h"

namespace Anggur
{
	Random::Random() : generator(device())
	{
	}

	float Random::Generate()
	{
		return generator();
	}

	float Random::Range(float lower, float upper)
	{
		std::uniform_real_distribution<float> distribution(lower, upper);
		return distribution(generator);
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
		generator.seed(n);
	}
}
