RWTexture2D<float> outputHeightmap : register(u0);

#define MAX_UINT 4294967295u

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


float ValueNoise(float2 position)
{
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
    
    float bottom = lerp(
            corners[0],
            corners[1],
            t.x
        );

    float top = lerp(
            corners[2],
            corners[3],
            t.x
        );

    return lerp(
            bottom,
            top,
            t.y
        );
}

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

    float total = 0.0f;
    float freq = frequency;
    float amp = 1.0f;
    float amplitudeSum = 0.0f;
    for (int i = 0; i < iterations; i++)
    {
        float2 noisePos = uv * freq ;
        float noise = ValueNoise(noisePos);
        noise = noise * 2.0f - 1.0f;

        total += noise * amp;
        amplitudeSum += amp;
        
        freq = freq * frequencyFactor;
        amp = amp * amplitudeFactor;
    }
    
    outputHeightmap[dispatchThreadID.xy] = heightFactor * total / max(amplitudeSum, 0.0001f);
}
