#include "ConstantBuffers.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

Texture2D<float> terrainHeightmap : register(t0);
Texture2D<float4> terrainNormalmap : register(t1);
SamplerState terrainSampler : register(s0);

VertexToPixel main(VertexInput input)
{
    VertexToPixel output;
    float2 uv = input.position.xz / 20.0f + 0.5f;
    
    float terrainHeight = terrainHeightmap.SampleLevel(terrainSampler, uv, 0.0f);

    float4 displacedPositionOS = float4(input.position, 1.0f);
    displacedPositionOS.y = terrainHeight;

    float4 displacedPositionWS = mul(displacedPositionOS, model);

    output.positionWS = displacedPositionWS.xyz;
    output.positionCS = mul(displacedPositionWS, viewProjection);

    output.normalWS = normalize(
        mul(
            terrainNormalmap.SampleLevel(terrainSampler, uv, 0.0f),
            modelInverseTranspose
        ).xyz
    );
    
    output.uv = uv;

    return output;
}
