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


Texture2D windNoise : register(t0);
SamplerState windSampler : register(s0);

VertexToPixel main(VSInput input)
{
    VertexToPixel output;

    const float4x4 instanceTransform = float4x4(
        input.instanceTransform0,
        input.instanceTransform1,
        input.instanceTransform2,
        input.instanceTransform3
    );

    // Imported mesh/node transform followed by the per-instance transform.
    const float4x4 modelToWorld = mul(
        model,
        instanceTransform
    );

    const float4 positionOS = float4(
        input.position,
        1.0f
    );

    float heightMask = saturate(1 - input.texcoord.y);
    const float bendMask = heightMask * heightMask;
    
    float phase =
    time * 2.0f +
    input.position.x * 0.15f +
    input.position.z * 0.12f;
    float wave = sin(phase);
    
    float4 positionWS = mul(
        positionOS,
        modelToWorld
    );

    float3 bladeOriginWS = mul(
        float4(0.0f, 0.0f, 0.0f, 1.0f),
        modelToWorld
    ).xyz;
    
    float2 noiseUV = (bladeOriginWS.xz - 10.0f) * 0.08 + time * 0.05;
    const float noise = windNoise.SampleLevel(windSampler, noiseUV, 0.0f).r * 2.0f - 1.0f;

    positionWS.xz += noise * bendMask * 0.3;

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