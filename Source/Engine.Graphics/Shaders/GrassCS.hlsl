#include "ShaderStructures.hlsli"

AppendStructuredBuffer<GrassInstance> grassInstances : register(u0);

cbuffer GrassGenerationConstants : register(b0)
{
    uint candidateCount;
    float chunkSize;
    float2 chunkPosition;
};

uint Hash(uint state)
{
    state = state * 747796405u + 2891336453u;

    uint word =
        ((state >> ((state >> 28u) + 4u)) ^ state)
        * 277803737u;

    return (word >> 22u) ^ word;
}

float Random01(uint seed)
{
    return Hash(seed) * (1.0f / 4294967296.0f);
}

[numthreads(256, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint id = dispatchThreadID.x;

    if (id >= candidateCount)
    {
        return;
    }

    // Makes the same candidate ID different between chunks.
    uint chunkSeed = Hash(
        asuint(chunkPosition.x) ^
        Hash(asuint(chunkPosition.y))
    );

    uint seed = Hash(
        id ^ chunkSeed
    );

    float randomX =
        Random01(seed ^ 0xA341316Cu);

    float randomZ =
        Random01(seed ^ 0xC8013EA4u);

    float randomRotation =
        Random01(seed ^ 0xAD90777Du);

    float randomScale =
        Random01(seed ^ 0x7E95761Eu);


    GrassInstance instance;

    instance.position.x = lerp(
        -chunkSize * 0.5f,
         chunkSize * 0.5f,
         randomX
    );

    instance.position.y = lerp(
        -chunkSize * 0.5f,
         chunkSize * 0.5f,
         randomZ
    );

    instance.rotation =
        randomRotation * 6.28318530718f;

    instance.scale = lerp(
        0.6f,
        1.4f,
        randomScale
    );

    grassInstances.Append(instance);
}