#pragma once
#include "../Bindable.h"
#include "ConstantBuffers.h"
#include <DirectXMath.h>

struct alignas(16) MaterialConstants
{
    DirectX::XMFLOAT3 baseColor = {0.85f, 0.11f, 0.85f};

    float metallic = 0.1f;
    float roughness = 0.5f;

    DirectX::XMFLOAT3 padding = {};
};

class Material : public Bindable
{
  public:
    Material(Graphics& gfx, std::string materialName, const MaterialConstants& constants = {});

    void Bind(Graphics& gfx) noexcept override;
    static std::shared_ptr<Material>
    Resolve(Graphics& gfx, std::string materialName, const MaterialConstants& constants = {});

    template <typename... Ignore> static std::string GenerateUID(const std::string& tag, Ignore&&... ignore)
    {
        return GenerateUID_(tag);
    }

    std::string GetUID() const noexcept override;

  private:
    static std::string GenerateUID_(const std::string& tag);

    std::string materialName;
    std::unique_ptr<PixelConstantBuffer<MaterialConstants>> pConstant;
};
