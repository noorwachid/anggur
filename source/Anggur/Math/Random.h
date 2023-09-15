#pragma once

#include <random>

namespace Anggur {
	class Random {
	public:
		// Getters

		static float generate();

		static float range(float lower, float upper);

		static float getNormal();

		static float getSigned();

		static float getAngle();

		static bool getBoolean();

		// Setters

		static void setSeed(float n);

	private:
		Random();

		static Random& getInstance() {
			static Random instance;

			return instance;
		}

	private:
		std::random_device _device;
		std::mt19937 _generator;
	};
}
