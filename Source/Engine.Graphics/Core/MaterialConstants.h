#pragma once

#include <DirectXMath.h>

struct alignas(16) MaterialConstants
{
    DirectX::XMFLOAT3 baseColor;
    float metallic;

    float roughness;
    DirectX::XMFLOAT3 padding;
};