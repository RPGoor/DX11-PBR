#include "ShaderStructures.hlsli"

AppendStructuredBuffer<GrassInstance> grassInstances : register(u0);

Texture2D<float4> terrainNormalmap : register(t1);
SamplerState terrainSampler : register(s0);

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

    float2 uv = instance.position / chunkSize + 0.5f;
    float3 normal = normalize(terrainNormalmap.SampleLevel(terrainSampler, uv, 0.0f).xyz);
    float slope = 1.0f - saturate(normal.y);
    float suitability = 1.0f - smoothstep(0.1f, 0.3f, slope);
    float placementRandom = Random01(seed ^ 0xB5297A4Du);
    if (placementRandom > suitability)
    {
        return;
    }
    
    float randomRotation =
        Random01(seed ^ 0xAD90777Du);

    float randomScale =
        Random01(seed ^ 0x7E95761Eu);

    instance.rotation =
        randomRotation * 6.28318530718f;

    instance.scale = lerp(
        0.6f,
        1.4f,
        randomScale
    );
    
    grassInstances.Append(instance);
}