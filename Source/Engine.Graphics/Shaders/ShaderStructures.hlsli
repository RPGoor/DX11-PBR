#ifndef SHADER_STRUCTS_INCL
#define SHADER_STRUCTS_INCL

struct VertexToPixel
{
    float4 positionCS : SV_Position;
    float3 positionWS : POSITION0;
    float3 normalWS : NORMAL0;
    float2 uv : TEXCOORD;
    float3 colorRand : COLOR;
};

struct SurfaceData
{
    float3 baseColor;
    float3 normalWS;

    float metallic;
    float roughness;
};

struct DirectionalLight
{
    float3 directionWS;
    float intensity;

    float3 color;
    float padding;
};

#endif