#ifndef CONST_BUFFERS_INCL
#define CONST_BUFFERS_INCL

#include "ShaderStructures.hlsli"


cbuffer FrameConstants : register(b0)
{
    row_major matrix view;
    row_major matrix projection;
    row_major matrix viewProjection;

    float3 cameraPositionWS;
    float time;
};

cbuffer ObjectConstants : register(b1)
{
    row_major matrix model;
    row_major matrix modelInverseTranspose;
};

cbuffer MaterialConstants : register(b2)
{
    float3 baseColor;
    float metallic;

    float roughness;
    float3 materialPadding;
};


cbuffer DirLight : register(b3)
{
    DirectionalLight directionalLight;
};

#endif