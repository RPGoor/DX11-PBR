#include "ConstantBuffers.hlsli"
#include "DirectionalLight.hlsli"
#include "Fog.hlsli"

Texture2D<float3> rockTexture : register(t0);
Texture2D<float3> dirtTexture : register(t1);
SamplerState terrainSampler : register(s0);

float4 main(VertexToPixel input) : SV_Target
{
    float slope = 1.0f - saturate(input.normalWS.y);
    float suitability = 1.0f - smoothstep(0.1f, 0.3f, slope);
    float3 dirtColor = dirtTexture.Sample(terrainSampler, input.positionWS.xz);
    float3 rockColor = rockTexture.Sample(terrainSampler, input.positionWS.xz);
    
    float3 texColor = lerp(rockColor, dirtColor, suitability);

    SurfaceData surface;
    //surface.baseColor = baseColor;
    surface.baseColor = texColor;
    surface.normalWS = normalize(input.normalWS);
    surface.metallic = saturate(max(metallic, 0.04f));
    surface.roughness = saturate(max(roughness, 0.04f));

    float3 color = EvaluateDirectionalLight(
        surface,
        cameraPositionWS,
        input.positionWS,
        directionalLight
    );
    
    color = ApplyDistanceFog(color, input.positionWS, cameraPositionWS);
    return float4(color, 1.0f);
}
