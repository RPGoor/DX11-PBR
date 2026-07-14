#ifndef PBR_RENDER_INCL
#define PBR_RENDER_INCL

#include "ShaderStructures.hlsli"

static const float PI = 3.14159265359f;
static const float MIN_ROUGHNESS = 0.045f;
static const float DIELECTRIC_F0 = 0.04f;

float Pow5(float value)
{
    float valueSquared = value * value;
    return valueSquared * valueSquared * value;
}

float3 FresnelSchlick(float viewDotHalfway, float3 f0)
{
    float fresnelFactor = Pow5(
        1.0f - saturate(viewDotHalfway)
    );

    return f0 + (1.0f - f0) * fresnelFactor;
}

float DistributionGGX(float normalDotHalfway, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSquared = alpha * alpha;

    float denominator =
        normalDotHalfway * normalDotHalfway *
        (alphaSquared - 1.0f) + 1.0f;

    denominator = PI * denominator * denominator;

    return alphaSquared / max(denominator, 0.000001f);
}

float GeometrySchlickGGX(float normalDotDirection, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;

    return normalDotDirection /
        max(
            normalDotDirection * (1.0f - k) + k,
            0.000001f
        );
}

float GeometrySmith(
    float normalDotView,
    float normalDotLight,
    float roughness)
{
    float viewGeometry =
        GeometrySchlickGGX(normalDotView, roughness);

    float lightGeometry =
        GeometrySchlickGGX(normalDotLight, roughness);

    return viewGeometry * lightGeometry;
}

struct PBRResult
{
    float3 diffuse;
    float3 specular;
};

PBRResult EvaluatePBR(
    SurfaceData surface,
    float3 viewDirectionWS,
    float3 lightDirectionWS)
{
    PBRResult result;

    float3 N = normalize(surface.normalWS);
    float3 V = normalize(viewDirectionWS);
    float3 L = normalize(lightDirectionWS);
    float3 H = normalize(V + L);

    float normalDotView = saturate(dot(N, V));
    float normalDotLight = saturate(dot(N, L));
    float normalDotHalfway = saturate(dot(N, H));
    float viewDotHalfway = saturate(dot(V, H));

    float roughness = max(
        surface.roughness,
        MIN_ROUGHNESS
    );

    float3 dielectricF0 =
        float3(DIELECTRIC_F0, DIELECTRIC_F0, DIELECTRIC_F0);

    float3 f0 = lerp(
        dielectricF0,
        surface.baseColor,
        surface.metallic
    );

    float3 fresnel = FresnelSchlick(
        viewDotHalfway,
        f0
    );

    float distribution = DistributionGGX(
        normalDotHalfway,
        roughness
    );

    float geometry = GeometrySmith(
        normalDotView,
        normalDotLight,
        roughness
    );

    float denominator =
        4.0f * normalDotView * normalDotLight;

    result.specular =
        distribution * geometry * fresnel /
        max(denominator, 0.000001f);

    float3 specularContribution = fresnel;
    float3 diffuseContribution = 1.0f - specularContribution;

    diffuseContribution *= 1.0f - surface.metallic;

    result.diffuse =
        diffuseContribution *
        surface.baseColor /
        PI;

    return result;
}

#endif