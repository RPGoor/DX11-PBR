#include "Material.h"
#include "../DX11/Bindings/ConstantBuffers.h"

Material::Material(
    Graphics& gfx,
    std::string materialName,
    const MaterialConstants& constants)
    : materialName(materialName)
{

    pConstant = std::make_unique<PixelConstantBuffer<MaterialConstants>>(gfx, constants, 2u);
}

void Material::Bind(Graphics& gfx) noexcept
{
    pConstant->Bind(gfx);
}

std::shared_ptr<Material> Material::Resolve(Graphics& gfx, std::string materialName, const MaterialConstants& constants)
{
    return Codex::Resolve<Material>(gfx, materialName, constants);
}

std::string Material::GenerateUID_(const std::string& tag)
{
    using namespace std::string_literals;
    return typeid(Material).name() + "#"s + tag;
}

std::string Material::GetUID() const noexcept
{
    return GenerateUID_(materialName);
}

