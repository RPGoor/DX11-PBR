#include "ConstantBuffers.hlsli"
#include "MathHelper.hlsli"

static const float ChunkSize = 20.0f;

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    uint instanceID : SV_InstanceID;
};

StructuredBuffer<GrassInstance> grassInstances : register(t2);

cbuffer GrassControls : register(b2)
{
    float bendStrength;
    float bendMaskPow;
    float speed;
    float2 direction;
    float uvScale;
};

Texture2D<float> terrainHeightmap : register(t0);
SamplerState terrainSampler : register(s0);

Texture2D windNoise : register(t1);
SamplerState windSampler : register(s1);

float SampleTerrainHeight(float2 position);
float GetNoiseValue(float3 bladeOrigin, float2 direction);

VertexToPixel main(VSInput input)
{
    GrassInstance instance = grassInstances[input.instanceID];

    VertexToPixel output;

    float terrainHeight = SampleTerrainHeight(instance.position);;
    float3 position = input.position * instance.scale;
    position.xz = Rotate(position.xz, instance.rotation);
    position.xz += instance.position;
    position.y += terrainHeight;
    float4 positionWS = mul(float4(position, 1.0f), model);


    // ---------------------------- WIND ----------------------------
    
    float heightMask = saturate(input.texcoord.y);
    float bendMask = pow(heightMask, bendMaskPow);

    float2 windDirection = all(direction == float2(0.0f, 0.0f)) ? float2(1.0f, 0.0f) : normalize(direction);

    float3 bladeOriginWS = mul(float4(instance.position.x, terrainHeight, instance.position.y, 1.0f), model).xyz;
    float noise = GetNoiseValue(bladeOriginWS, windDirection);
    float bendAngle = noise * bendMask * radians(45.0f) * bendStrength;
    
    float3 offset = positionWS.xyz - bladeOriginWS;
    float3 bendAxis = normalize(cross(float3(0.0f, 1.0f, 0.0f), float3(windDirection.x, 0.0f, windDirection.y)));
    float3 bentOffset = RotateAroundAxis(offset, bendAxis, bendAngle);
    positionWS = float4(bladeOriginWS + bentOffset, 1.0f);


    // --------------------- RECALCULATE NORMAL ---------------------
    
    float3 normal = input.normal;
    normal.xz = Rotate(normal.xz, instance.rotation);
    float3 normalWS = normalize(mul(normal, (float3x3) model));
    float3 widthTangentWS = normalize(cross(float3(0.0f, 1.0f, 0.0f), normalWS));
    normalWS = normalize(RotateAroundAxis(normalWS, bendAxis, bendAngle));

    
    // ------------------- VIEW-BASED THICKENING -------------------

    float4 positionVS = mul(positionWS, view);
    float3 normalVS = normalize(mul(float4(normalWS, 0.0f), view).xyz);
    float3 viewDirectionVS = normalize(-positionVS.xyz);
    float NdotV = abs(dot(normalVS, viewDirectionVS));
    float edgeMask = smoothstep(0.3f, 0.9f, 1.0f - saturate(NdotV));
    float2 widthDirectionVS = normalVS.xy;
    float widthLengthSq = dot(widthDirectionVS, widthDirectionVS);
    widthDirectionVS *= rsqrt(max(widthLengthSq, 1e-8f));
    float side = sign(input.position.x);
    static const float ViewThickness = 0.01f;
    positionVS.xy += widthDirectionVS * side * edgeMask * ViewThickness;

    // ---------------------- ROUNDED NORMALS ----------------------
    
    float roundSide = input.texcoord.x * 2.0f - 1.0f;
    float3 shadingNormalWS = normalize(normalWS + widthTangentWS * (roundSide * 0.65f));



    output.positionWS = positionWS.xyz;
    output.positionCS = mul(positionVS, projection);
    output.normalWS = shadingNormalWS;
    output.uv = input.texcoord;
    return output;
}

float SampleTerrainHeight(float2 position)
{
    float2 terrainUV = position / ChunkSize + 0.5f;
    return terrainHeightmap.SampleLevel(terrainSampler, terrainUV, 0.0f);
}

float GetNoiseValue(float3 position, float2 direction)
{
    float2 noiseUV = position.xz * uvScale / 100.0f + direction * time * speed / 100.0f;
    return windNoise.SampleLevel(windSampler, noiseUV, 0.0f).r * 2.0f - 1.0f;
}