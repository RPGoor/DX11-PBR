RWTexture2D<float> outputHeightmap : register(u0);
RWTexture2D<float4> outputNormalmap : register(u1);

#define MAX_UINT 4294967295u

struct ValueNoiseResult
{
    float value;
    float2 gradient;
};

cbuffer TerrainGenerationCBuf : register(b0)
{
    uint textureDimensions;
    float frequency;
    float heightFactor;
    float frequencyFactor;
    float amplitudeFactor;
    uint iterations;
};

uint Convert2D(uint2 input)
{
    return
    input.x * 73856093u ^
    input.y * 19349663u;
}

float NormalizeUint(uint input)
{
    return float(input) / float(MAX_UINT);
}

float Hash(uint2 input)
{
    uint hashedInput = Convert2D(input);
    
    uint state = hashedInput * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return NormalizeUint((word >> 22u) ^ word);
}


ValueNoiseResult ValueNoise(float2 position)
{
    ValueNoiseResult result;
    uint2 corner = floor(position);
    float2 local = frac(position);
    
    float corners[4] =
    {
        Hash(corner),
            Hash(corner + uint2(1, 0)),
            Hash(corner + uint2(0, 1)),
            Hash(corner + uint2(1, 1)),
    };


    float2 t = local * local * (3.0f - 2.0f * local);
    float2 dt = 6.0f * local * (1.0f - local);

    float bottom = lerp(corners[0], corners[1], t.x);
    float top = lerp(corners[2], corners[3], t.x);

    float difX = dt.x * lerp(corners[1] - corners[0], corners[3] - corners[2], t.y);
    float difY = dt.y * (top - bottom);

    result.value = lerp(bottom, top, t.y);
    result.gradient = float2(difX, difY);
    return result;
}

[numthreads(8, 8, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    if (dispatchThreadID.x >= textureDimensions ||
        dispatchThreadID.y >= textureDimensions)
    {
        return;
    }

    float2 uv = float2(dispatchThreadID.xy) / float(textureDimensions - 1u);

    float height = 0.0f;
    float2 gradient = float2(0.0f, 0.0f);
    float freq = frequency;
    float amp = 1.0f;
    float amplitudeSum = 0.0f;
    for (int i = 0; i < iterations; i++)
    {
        float2 noisePos = uv * freq ;
        ValueNoiseResult noise = ValueNoise(noisePos);
        noise.value = noise.value * 2.0f - 1.0f;
        
        height += noise.value * amp;
        gradient += noise.gradient * amp * freq;
        
        amplitudeSum += amp;
        
        freq = freq * frequencyFactor;
        amp = amp * amplitudeFactor;
    }
    
    gradient = heightFactor * gradient / max(amplitudeSum, 0.0001f);
    float3 normal = normalize(float3(-gradient.x, 1, -gradient.y));

    outputHeightmap[dispatchThreadID.xy] = heightFactor * height / max(amplitudeSum, 0.0001f);
    outputNormalmap[dispatchThreadID.xy] = float4(normal, 0.0f);
}
