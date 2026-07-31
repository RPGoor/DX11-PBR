RWTexture2D<float> outputHeightmap : register(u0);

cbuffer TerrainGenerationCBuf : register(b0)
{
    uint textureDimensions;
};

[numthreads(8, 8, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    if (dispatchThreadID.x >= textureDimensions ||
        dispatchThreadID.y >= textureDimensions)
    {
        return;
    }

    float2 uv =
        float2(dispatchThreadID.xy) /
        float(textureDimensions - 1u);

    outputHeightmap[dispatchThreadID.xy] = uv.x;
}