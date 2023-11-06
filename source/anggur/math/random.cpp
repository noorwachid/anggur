#include "anggur/math/random.h"
#include "anggur/math.h"

namespace Anggur {
	Random::Random() : generator(device()) {
	}

	float Random::generate() {
		return getInstance().generator();
	}

	float Random::range(float lower, float upper) {
		std::uniform_real_distribution<float> distribution(lower, upper);
		return distribution(getInstance().generator);
	}

	float Random::getNormal() {
		return range(0.f, 1.f);
	}

	float Random::getSigned() {
		return range(-1.f, 1.f);
	}

	float Random::getAngle() {
		return range(0.f, Math::twoPi);
	}

	bool Random::getBoolean() {
		return getNormal() > 0.5;
	}

	void Random::setSeed(float n) {
		getInstance().generator.seed(n);
	}
}
