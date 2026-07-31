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

    float3 displacedPosition = input.position;
    displacedPosition.y += terrainHeight;
    
    float4 positionOS = float4(displacedPosition, 1.0f);

    float4 positionWS = mul(positionOS, model);

    output.positionWS = positionWS.xyz;
    output.positionCS = mul(positionWS, viewProjection);

    output.normalWS = normalize(
        mul(
            float4(input.normal, 0.0f),
            modelInverseTranspose
        ).xyz
    );
    
    output.uv = input.texcoord;

    return output;
}