#include "ConstantBuffers.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

VertexToPixel main(VertexInput input)
{
    VertexToPixel output;

    float4 positionOS = float4(input.position, 1.0f);

    float4 positionWS = mul(positionOS, model);

    output.positionWS = positionWS.xyz;
    output.positionCS = mul(positionWS, viewProjection);

    output.normalWS = normalize(
        mul(
            float4(input.normal, 0.0f),
            modelInverseTranspose
        ).xyz
    );
    output.uv = input.texcoord;
    return output;
}
