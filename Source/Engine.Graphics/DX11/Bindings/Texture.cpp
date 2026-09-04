#include "Texture.h"
#include "../BindableCodex.h"
#include "../GraphicsExceptionsMacros.h"

#include <directxtk/WICTextureLoader.h>
#include <filesystem>

namespace wrl = Microsoft::WRL;

Texture::Texture(Graphics& gfx, const std::string& path, UINT slot)
    : path(path),
      slot(slot)
{
    INFOMAN(gfx);

    const std::wstring widePath = std::filesystem::path(path).wstring();

    GFX_THROW_INFO(
        DirectX::CreateWICTextureFromFile(
            GetDevice(gfx),
            GetContext(gfx),
            widePath.c_str(),
            nullptr,
            pTextureView.GetAddressOf()
        )
    );
}

void Texture::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
}

void Texture::BindVS(Graphics& gfx) noexcept
{
    GetContext(gfx)->VSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
}

std::shared_ptr<Texture> Texture::Resolve(Graphics& gfx, const std::string& path, UINT slot)
{
    return Codex::Resolve<Texture>(gfx, path, slot);
}

std::string Texture::GenerateUID(const std::string& path, UINT slot)
{
    using namespace std::string_literals;
    return typeid(Texture).name() + "#"s + path + "#" + std::to_string(slot);
}

std::string Texture::GetUID() const noexcept
{
    return GenerateUID(path, slot);
}
