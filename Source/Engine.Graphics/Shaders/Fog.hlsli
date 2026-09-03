#ifndef FOG_INCL
#define FOG_INCL

float3 ApplyDistanceFog(float3 color, float3 positionWS, float3 cameraPositionWS)
{
    float dist = distance(positionWS, cameraPositionWS);
    float fogAlpha = saturate((dist - 55.0f) / 15.0f);
    return lerp(color, float3(0.72f, 0.80f, 0.84f), fogAlpha);
}

#endif