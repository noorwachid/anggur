#include "anggur/math/random.h"
#include "anggur/math.h"

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

	float Random::Normal()
	{
		return Range(0.f, 1.f);
	}

	float Random::Signed()
	{
		return Range(-1.f, 1.f);
	}

	float Random::Angle()
	{
		return Range(0.f, Math::twoPi);
	}

	bool Random::Boolean()
	{
		return Normal() > 0.5;
	}

	void Random::SetSeed(float n)
	{
		GetInstance()._generator.seed(n);
	}
}
