#pragma once

#include <random>
#include "Math.h"

namespace Anggur 
{
	class RandomGenerator 
	{
	public:
		RandomGenerator(): generator(device()) 
		{
		}

		void SetSeed(float n) 
		{
			generator.seed(n);
		}

		float Generate() 
		{
			return generator();
		}

		float Range(float lower, float upper) 
		{
			std::uniform_real_distribution<float> distribution(lower, upper);
			return distribution(generator);
		}

	private:
		std::random_device device;
		std::mt19937 generator;

	};

	class Random {
	public:
		static float Generate() 
		{
			return generator.Generate();
		}

		static float Range(float lower, float upper) 
		{
			return generator.Range(lower, upper);
		}

		static float GetNormal() 
		{
			return generator.Range(0.f, 1.f);
		}

		static float GetSigned() 
		{
			return generator.Range(-1.f, 1.f);
		}

		static float GetAngle() 
		{
			return generator.Range(0.f, Math::twoPi);
		}

		static bool GetBoolean() 
		{
			return GetNormal() > 0.5;
		}


	private:
		static RandomGenerator generator;
	};
}
