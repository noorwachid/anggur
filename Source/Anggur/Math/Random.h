#pragma once

#include "Anggur/Math/Math.h"
#include <random>

namespace Anggur
{
	class Random
	{
	public:
		// Getters

		static float Generate();

		static float Range(float lower, float upper);

		static float GetNormal();

		static float GetSigned();

		static float GetAngle();

		static bool GetBoolean();

		// Setters

		static void SetSeed(float n);

	private:
		Random();

		static Random& GetInstance() 
		{
			static Random instance;

			return instance;
		}

	  private:
		std::random_device device;
		std::mt19937 generator;
	};
}
