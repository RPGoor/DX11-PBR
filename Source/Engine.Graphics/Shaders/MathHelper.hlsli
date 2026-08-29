#ifndef MATH_HELPER_INCL
#define MATH_HELPER_INCL

float2 Rotate(float2 position, float rotation)
{
    float sinRotation;
    float cosRotation;
    sincos(rotation, sinRotation, cosRotation);

    return float2(
        position.x * cosRotation - position.y * sinRotation,
        position.x * sinRotation + position.y * cosRotation
    );
}

float3 RotateAroundAxis(float3 vect, float3 axis, float angle)
{
    float sinA, cosA;
    sincos(angle, sinA, cosA);
    return vect * cosA + cross(axis, vect) * sinA + axis * dot(axis, vect) * (1.0f - cosA);
}

#endif