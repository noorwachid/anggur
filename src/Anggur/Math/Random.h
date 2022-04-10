#pragma once

#include <random>
#include "Math.h"

namespace Anggur {
	class RandomGenerator {
	public:
		RandomGenerator()
			:
			generator(device()) {
		}

		void setSeed(float n) {
			generator.seed(n);
		}

		float generate() {
			return generator();
		}

		float range(float lower, float upper) {
			std::uniform_real_distribution<float> distribution(lower, upper);
			return distribution(generator);
		}

	private:
		std::random_device device;
		std::mt19937 generator;

	};

	class Random {
	public:
		static float generate() {
			return generator.generate();
		}

		static float range(float lower, float upper) {
			return generator.range(lower, upper);
		}

		static float getNormal() {
			return generator.range(0.f, 1.f);
		}

		static float getSigned() {
			return generator.range(-1.f, 1.f);
		}

		static float getAngle() {
			return generator.range(0.f, Math::twoPi);
		}

		static bool getBoolean() {
			return getNormal() > 0.5;
		}


	private:
		static RandomGenerator generator;
	};
}
