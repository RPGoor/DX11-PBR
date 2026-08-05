#include "ConstantBuffers.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

Texture2D<float> terrainHeightmap : register(t0);
SamplerState terrainSampler : register(s0);

VertexToPixel main(VertexInput input)
{
    VertexToPixel output;

    float terrainHeight = terrainHeightmap.SampleLevel(
        terrainSampler,
        input.texcoord,
        0.0f
    );

    float4 displacedPositionOS = float4(input.position, 1.0f);
    displacedPositionOS.y = terrainHeight;

    float4 displacedPositionWS = mul(displacedPositionOS, model);

    output.positionWS = displacedPositionWS.xyz;
    output.positionCS = mul(displacedPositionWS, viewProjection);

    output.normalWS = normalize(
        mul(
            float4(input.normal, 0.0f),
            modelInverseTranspose
        ).xyz
    );
    
    output.uv = input.texcoord;

    return output;
}