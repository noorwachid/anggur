#include "Random.h"

namespace Anggur {
	Random::Random(): generator(device()) {
	}

	float Random::generate() {
		return generator();
	}

	float Random::getRange(float lower, float upper) {
		std::uniform_real_distribution<float> distribution(lower, upper);
		return distribution(generator);
	}

	float Random::getNormal() {
		return getRange(0.f, 1.f);
	}

	float Random::getSigned() {
		return getRange(-1.f, 1.f);
	}

	float Random::getAngle() {
		return getRange(0.f, Math::twoPi);
	}

	bool Random::getBoolean() {
		return getNormal() > 0.5;
	}
	
	void Random::setSeed(float n) {
		generator.seed(n);
	}
}
