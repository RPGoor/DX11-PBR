#pragma once
#include "../DX11/Bindings/Bindable.h"
#include <DirectXMath.h>

struct alignas(16) MaterialConstants
{
    DirectX::XMFLOAT3 baseColor = {
        0.85f, 0.11f, 0.85f
    };

    float metallic = 0.1f;
    float roughness = 0.5f;

    DirectX::XMFLOAT3 padding = {};
};

class Material : Bindable
{
public:
    Material(Graphics& gfx, const MaterialConstants& constants = { });

    void Bind(Graphics& gfx) noexcept override;

    //void Add(std::shared_ptr<Bindable> bindable);

private:
    std::vector<std::shared_ptr<Bindable>> bindables;
};
