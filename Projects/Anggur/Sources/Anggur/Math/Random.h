#pragma once

#include <random>
#include "Math.h"

namespace Anggur {
	class Random {
	public:
		Random();

		// Getters

		float generate();

		float range(float lower, float upper);

		float getNormal();

		float getSigned();

		float getAngle();

		bool getBoolean();

		// Setters

		void setSeed(float n);

	private:
		std::random_device device;
		std::mt19937 generator;
	};
}
