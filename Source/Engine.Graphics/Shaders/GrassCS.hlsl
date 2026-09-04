#include "ShaderStructures.hlsli"
#include "Random.hlsli"

AppendStructuredBuffer<GrassInstance> grassInstances : register(u0);

Texture2D<float4> terrainNormalmap : register(t1);
SamplerState terrainSampler : register(s0);

cbuffer GrassGenerationConstants : register(b0)
{
    uint candidateCount;
    float chunkSize;
    float2 chunkPosition;
};

[numthreads(256, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint id = dispatchThreadID.x;

    if (id >= candidateCount)
    {
        return;
    }

    uint chunkSeed = Hash(uint2(asuint(chunkPosition.x), asuint(chunkPosition.y)));
    uint seed = Hash(uint2(id, chunkSeed));

    GrassInstance instance;

    float halfChunkSize = chunkSize * 0.5f;
    instance.position.x = lerp(-halfChunkSize, halfChunkSize, NormalizedHash(uint2(seed, 0u)));
    instance.position.y = lerp(-halfChunkSize, halfChunkSize, NormalizedHash(uint2(seed, 1u)));

    float2 uv = instance.position / chunkSize + 0.5f;
    float3 normal = normalize(terrainNormalmap.SampleLevel(terrainSampler, uv, 0.0f).xyz);
    float slope = 1.0f - saturate(normal.y);
    float suitability = 1.0f - smoothstep(0.1f, 0.3f, slope);
    
    float placementHash = NormalizedHash(uint2(seed, 2u));
    if (placementHash > suitability)
    {
        return;
    }

    instance.rotation = NormalizedHash(uint2(seed, 3u)) * 6.28318530718f;
    instance.scale = lerp(0.6f, 1.4f, NormalizedHash(uint2(seed, 4u)));
    grassInstances.Append(instance);
}
