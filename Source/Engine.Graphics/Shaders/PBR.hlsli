#ifndef PBR_RENDER_INCL
#define PBR_RENDER_INCL

#include "ShaderStructures.hlsli"

static const float PI = 3.14159265359f;

float3 FresnelSchlick(float VdotH, float3 color, float3 metallic)
{
    float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), color, metallic);
    float fresnelFactor = pow(1.0f - saturate(VdotH), 5);
    return f0 + (1.0f - f0) * fresnelFactor;
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSquared = alpha * alpha;

    float NdotH = saturate(dot(N, H));
    float NdotHSquared = NdotH * NdotH;
    float denominator = NdotHSquared * (alphaSquared - 1.0f) + 1.0f;

    denominator = PI * denominator * denominator;
    return alphaSquared / max(denominator, 0.000001f);
}

float GeometricAttenuationK(float NdotX, float k)
{
    return NdotX / max(NdotX * (1.0f - k) + k, 0.000001f);
}

float GeometricAttenuation(float NdotV, float NdotL, float roughness)
{
    float k = pow(roughness + 1.0f, 2)/8.0f;
    float Gv = GeometricAttenuationK(NdotV, k);
    float Gl = GeometricAttenuationK(NdotL, k);
    return Gv * Gl;
}

struct PBRResult
{
    float3 diffuse;
    float3 specular;
};

PBRResult EvaluatePBR(
    float3 N,
    float3 V,
    float3 L,
    float3 color,
    float roughness,
    float metallic)
{
    PBRResult result;

    float3 H = normalize(V + L);

    float NdotV = saturate(dot(N, V));
    float NdotL = saturate(dot(N, L));
    float VdotH = saturate(dot(V, H));

    float D = DistributionGGX(N, H, roughness);
    float3 F = FresnelSchlick(VdotH, color, metallic);
    float G = GeometricAttenuation(NdotV, NdotL, roughness);

    float denominator = max(4.0f * NdotV * NdotL, 0.000001f);

    result.specular = D * G * F / denominator;

    float3 specularContribution = F;
    float3 diffuseFactor = (1.0f - specularContribution) * (1.0f - metallic);

    result.diffuse = diffuseFactor * color / PI;
    return result;
}

#endif