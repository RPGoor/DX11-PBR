#ifndef RANDOM_INCL
#define RANDOM_INCL

#define MAX_UINT 4294967295u

uint Combine(uint2 input)
{
    return
        input.x * 73856093u ^
        input.y * 19349663u;
}

uint Hash(uint input)
{
    uint state = input * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

uint Hash(uint2 input)
{
    uint hashedInput = Combine(input);
    return Hash(hashedInput);
}

float NormalizeUint(uint input)
{
    return float(input) / float(MAX_UINT);
}

float NormalizedHash(uint2 input)
{
    return NormalizeUint(Hash(input));
}

float NormalizedHash(uint input)
{
    return NormalizeUint(Hash(input));
}

#endif