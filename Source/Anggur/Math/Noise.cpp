#include "Noise.hpp"

namespace Anggur
{

Noise::Noise(Anggur::NoiseType type)
{
    setType(type);
}

void Noise::setType(NoiseType type)
{
    instance.SetNoiseType(static_cast<FastNoiseLite::NoiseType>(type));
}

void Noise::setFrequency(float frequency)
{
    instance.SetFrequency(frequency);
}

void Noise::setFractalOctaves(int octaves)
{
    instance.SetFractalOctaves(octaves);
}

void Noise::setSeed(int seed)
{
    instance.SetSeed(seed);
}

float Noise::get(float x, float y)
{
    return instance.GetNoise(x, y);
}

float Noise::get(float x, float y, float z)
{
    return instance.GetNoise(x, y, z);
}

}
