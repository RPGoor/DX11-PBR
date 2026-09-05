#include "Random.hlsli"

RWTexture2D<float> outputHeightmap : register(u0);
RWTexture2D<float4> outputNormalmap : register(u1);

cbuffer TerrainGenerationCBuf : register(b0)
{
    uint textureDimensions;
    float frequency;
    float heightFactor;
    float frequencyFactor;
    float amplitudeFactor;
    uint iterations;
    float2 chunkPosition;
    uint seed;
};

[numthreads(8, 8, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    if (dispatchThreadID.x >= textureDimensions ||
        dispatchThreadID.y >= textureDimensions)
    {
        return;
    }

    float2 uv = float2(dispatchThreadID.xy) / float(textureDimensions - 1u);
    float2 chunkOffset = chunkPosition / 20.0f;
    
    float height = 0.0f;
    float2 gradient = float2(0.0f, 0.0f);
    float freq = frequency;
    float amp = 1.0f;
    float amplitudeSum = 0.0f;
    for (uint i = 0; i < iterations; i++)
    {
        float2 noisePos = (uv + chunkOffset) * freq;
        ValueNoiseResult noise = ValueNoise(noisePos, seed);
        noise.value = noise.value * 2.0f - 1.0f;
        
        height += noise.value * amp;
        gradient += noise.gradient * 2.0 * amp * freq;
        
        amplitudeSum += amp;
        
        freq = freq * frequencyFactor;
        amp = amp * amplitudeFactor;
    }
    
    gradient = heightFactor * gradient / max(amplitudeSum, 0.0001f) / 20.0f;
    float3 normal = normalize(float3(-gradient.x, 1.0f, -gradient.y));

    outputHeightmap[dispatchThreadID.xy] = heightFactor * height / max(amplitudeSum, 0.0001f);
    outputNormalmap[dispatchThreadID.xy] = float4(normal, 0.0f);
}
