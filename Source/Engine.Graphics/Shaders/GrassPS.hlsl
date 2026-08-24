#include "ConstantBuffers.hlsli"
#include "DirectionalLight.hlsli"

float4 main(
    VertexToPixel input,
    bool isFrontFace : SV_IsFrontFace
) : SV_Target
{
    float3 normalWS = normalize(input.normalWS);

    if (!isFrontFace)
    {
        normalWS = -normalWS;
    }

    SurfaceData surface;
    surface.baseColor = baseColor;
    surface.normalWS = normalWS;
    surface.metallic = 0.0f;
    surface.roughness = roughness;

    float upFacing = saturate(normalWS.y * 0.5f + 0.5f);

    float3 skyColor = float3(0.18f, 0.28f, 0.16f);
    float3 ambient = skyColor * lerp(0.25f, 1.0f, upFacing);

    
    float3 color = EvaluateDirectionalLight(
        surface,
        cameraPositionWS,
        input.positionWS,
        directionalLight
    );

    color += ambient;

    color = saturate(color * (input.uv.y) * input.colorRand.rgb);

    return float4(color, 1.0f);
}