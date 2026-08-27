#include "ConstantBuffers.hlsli"

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;

    row_major matrix instanceTransform : INSTANCE_TRANSFORM;
    float4 instanceColorOffset : INSTANCE_COLOR_OFFSET;

};

cbuffer GrassControls : register(b2)
{
    float horizontalBendStrength;
    float verticalBendStrength;
    float bendMaskPow;
    float speed;
    float2 direction;
    float uvScale;
};

Texture2D<float> terrainHeightmap : register(t0);
SamplerState terrainSampler : register(s0);

Texture2D windNoise : register(t1);
SamplerState windSampler : register(s1);

VertexToPixel main(VSInput input)
{
    VertexToPixel output;
    
    float2 terrainUV = input.instanceTransform[3].xz / 20.0f + 0.5f;
    input.instanceTransform[3].y = terrainHeightmap.SampleLevel(terrainSampler, terrainUV, 0.0f);
    
    const matrix modelToWorld = mul(input.instanceTransform, model);

    float4 positionOS = float4(
        input.position,
        1.0f
    );

    float4 positionWS = mul(
        positionOS,
        modelToWorld
    );
    
    float heightMask = saturate(input.texcoord.y);
    const float bendMask = pow(heightMask, bendMaskPow);
    
    float3 bladeOriginWS = mul(
        float4(0.0f, 0.0f, 0.0f, 1.0f),
        modelToWorld
    ).xyz;
    
    float2 noiseUV = (bladeOriginWS.xz) * uvScale / 100.0f + direction * time * speed / 100.0f;
    const float noise = windNoise.SampleLevel(windSampler, noiseUV, 0.0f).r * 2.0f - 1.0f;

    positionWS.xz += noise * bendMask * horizontalBendStrength;
    positionWS.y -= abs(noise) * bendMask * verticalBendStrength;
    
    output.positionWS = positionWS.xyz;

    output.positionCS = mul(
        positionWS,
        viewProjection
    );

    output.normalWS = normalize(
        mul(
            float4(input.normal, 0.0f),
            modelToWorld
        ).xyz
    );

    output.uv = input.texcoord;
    output.colorRand = input.instanceColorOffset;
    return output;
}