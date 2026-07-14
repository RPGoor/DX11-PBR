#include "ConstantBuffers.hlsli"
#include "DirectionalLight.hlsli"

float4 main(VertexToPixel input) : SV_Target
{
    SurfaceData surface;

    surface.baseColor = baseColor;
    surface.normalWS = normalize(input.normalWS);
    surface.metallic = saturate(metallic);
    surface.roughness = saturate(roughness);

    float3 color = EvaluateDirectionalLight(
        surface,
        cameraPositionWS,
        input.positionWS,
        directionalLight
    );

    return float4(color, 1.0f);
}