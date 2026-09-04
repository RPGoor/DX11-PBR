#ifndef RANDOM_INCL
#define RANDOM_INCL

#include "ShaderStructures.hlsli"

#define MAX_UINT 4294967295u


uint Combine(uint2 input)
{
    return input.x * 73856093u ^ input.y * 19349663u;
}

uint2 Split(uint input)
{
    return uint2(input * 73856093u, input * 19349663u);
}

uint Hash(uint input)
{
    uint state = input * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

uint Hash(uint2 input)
{
    uint hashedInput = Combine(input);
    return Hash(hashedInput);
}

float NormalizeUint(uint input)
{
    return float(input) / float(MAX_UINT);
}

float NormalizedHash(uint2 input)
{
    return NormalizeUint(Hash(input));
}

float NormalizedHash(uint input)
{
    return NormalizeUint(Hash(input));
}

ValueNoiseResult ValueNoise(float2 position, uint seed)
{
    ValueNoiseResult result;
    uint2 corner = floor(position);
    float2 local = frac(position);
    uint2 splitSeed = Split(seed);
    float corners[4] =
    {
        NormalizedHash(corner ^ seed),
        NormalizedHash((corner + uint2(1, 0)) ^ seed),
        NormalizedHash((corner + uint2(0, 1)) ^ seed),
        NormalizedHash((corner + uint2(1, 1)) ^ seed),
    };

    float2 t = local * local * (3.0f - 2.0f * local);
    float2 dt = 6.0f * local * (1.0f - local);

    float bottom = lerp(corners[0], corners[1], t.x);
    float top = lerp(corners[2], corners[3], t.x);

    float difX = dt.x * lerp(corners[1] - corners[0], corners[3] - corners[2], t.y);
    float difY = dt.y * (top - bottom);

    result.value = lerp(bottom, top, t.y);
    result.gradient = float2(difX, difY);
    return result;
}

#endif
