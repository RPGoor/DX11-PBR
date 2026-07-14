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
    float3 viewDirectionWS = normalize(
        cameraPositionWS - surfacePositionWS
    );

    // The stored direction describes the direction the light travels.
    // Negating it gives the direction from the surface toward the light.
    float3 lightDirectionWS = normalize(
        -light.directionWS
    );

    float normalDotLight = saturate(
        dot(
            normalize(surface.normalWS),
            lightDirectionWS
        )
    );

    if (normalDotLight <= 0.0f)
    {
        return 0.0f;
    }

    PBRResult pbr = EvaluatePBR(
        surface,
        viewDirectionWS,
        lightDirectionWS
    );

    float3 radiance =
        light.color *
        light.intensity;

    return
        (pbr.diffuse + pbr.specular) *
        radiance *
        normalDotLight;
}

#endif