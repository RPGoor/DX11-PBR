#include "Material.h"
#include "../DX11/Bindings/ConstantBuffers.h"

Material::Material(
    Graphics& gfx,
    const MaterialConstants& constants
)
{
    bindables.push_back(
        std::make_shared<
        PixelConstantBuffer<MaterialConstants>>(gfx, constants, 2u)
    );
}

void Material::Bind(Graphics& gfx) noexcept
{
    for (const auto& bindable : bindables)
    {
        bindable->Bind(gfx);
    }
}