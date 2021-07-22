#pragma once

#include <FastNoiseLite.hpp>

namespace Anggur
{

enum class NoiseType
{
    OpenSimplex2  = FastNoiseLite::NoiseType_OpenSimplex2,
    OpenSimplex2S = FastNoiseLite::NoiseType_OpenSimplex2S,
    Cellular      = FastNoiseLite::NoiseType_Cellular,
    Perlin        = FastNoiseLite::NoiseType_Perlin,
    ValueCubic    = FastNoiseLite::NoiseType_ValueCubic,
    Value         = FastNoiseLite::NoiseType_Value
};

class Noise
{
public:
    Noise(NoiseType type = NoiseType::Perlin);

    void setType(NoiseType type);
    void setFrequency(float frequency);
    void setFractalOctaves(int octaves);

    void setSeed(int seed);

    float get(float x, float y);
    float get(float x, float y, float z);

private:
    FastNoiseLite instance;
};

}
