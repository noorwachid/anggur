#pragma once

#include <random>
#include "Math.hpp"

namespace Anggur {

class RandomGenerator
{
public:
    RandomGenerator():
        mGenerator(mDevice())
    {
    }

    void SetSeed(float n)
    {
        mGenerator.seed(n);
    }

    float Generate()
    {
        return mGenerator();
    }

    float Range(float lower, float upper)
    {
        std::uniform_real_distribution<float> distribution(lower, upper);
        return distribution(mGenerator);
    }

private:
    std::random_device mDevice;
    std::mt19937 mGenerator;

};

class Random
{
public:
    static float Generate()
    {
        return mGenerator.Generate();
    }

    static float Range(float lower, float upper)
    {
        return mGenerator.Range(lower, upper);
    }

    static float GetNormal()
    {
        return mGenerator.Range(0.f, 1.f);
    }

    static float GetSigned()
    {
        return mGenerator.Range(-1.f, 1.f);
    }

    static float GetAngle()
    {
        return mGenerator.Range(0.f, Math::TwoPi);
    }


private:
    static RandomGenerator mGenerator;

};

}
