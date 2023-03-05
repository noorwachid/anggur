#pragma once

#include "Anggur/Math/Math.h"
#include <random>

namespace Anggur
{
	class Random
	{
	  public:
		Random();

		// Getters

		float Generate();

		float Range(float lower, float upper);

		float GetNormal();

		float GetSigned();

		float GetAngle();

		bool GetBoolean();

		// Setters

		void SetSeed(float n);

	  private:
		std::random_device device;
		std::mt19937 generator;
	};
}
