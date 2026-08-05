#ifndef DIRECTIONAL_LIGHT_INCL
#define DIRECTIONAL_LIGHT_INCL

#include "ShaderStructures.hlsli"
#include "PBR.hlsli"

float3 EvaluateDirectionalLight(
    SurfaceData surface,
    float3 cameraPositionWS,
    float3 surfacePositionWS,
    DirectionalLight light)
{
    float3 N = normalize(surface.normalWS);

    float3 V = normalize(
        cameraPositionWS - surfacePositionWS
    );

    float3 L = normalize(
        -light.directionWS
    );

    float normalDotLight = saturate(
        dot(N,L)
    );

    PBRResult pbr = EvaluatePBR(N, V, L, surface.baseColor, surface.roughness, surface.metallic);

    float3 radiance =
        light.color *
        light.intensity;

    return
        (pbr.diffuse + pbr.specular) *
        radiance *
        normalDotLight;
}

#endif