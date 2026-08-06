#include "ConstantBuffers.hlsli"

struct VSInput
{
    float3 position : Position;
    float3 normal : Normal;
    float2 texcoord : Texcoord;

    float4 instanceTransform0 : InstanceTransform0;
    float4 instanceTransform1 : InstanceTransform1;
    float4 instanceTransform2 : InstanceTransform2;
    float4 instanceTransform3 : InstanceTransform3;
    float4 instanceTransform4 : InstanceTransform4;

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

    float4x4 instanceTransform = float4x4(
        input.instanceTransform0,
        input.instanceTransform1,
        input.instanceTransform2,
        input.instanceTransform3
    );
    
    float2 terrainUV = instanceTransform[3].xz / 20.0f + 0.5f;
    instanceTransform[3].y = terrainHeightmap.SampleLevel(terrainSampler, terrainUV, 0.0f);
    
    const float4x4 modelToWorld = mul(instanceTransform, model);

    float4 positionOS = float4(
        input.position,
        1.0f
    );

    float4 positionWS = mul(
        positionOS,
        modelToWorld
    );
    
    float heightMask = saturate(1 - input.texcoord.y);
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

    // Correct for rotation and uniform scale.
    output.normalWS = normalize(
        mul(
            float4(input.normal, 0.0f),
            modelToWorld
        ).xyz
    );

    output.uv = input.texcoord;
    output.colorRand = input.instanceTransform4;
    return output;
}