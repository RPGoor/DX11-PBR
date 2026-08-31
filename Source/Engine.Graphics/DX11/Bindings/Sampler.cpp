#include "Sampler.h"
#include "../GraphicsExceptionsMacros.h"
#include "../BindableCodex.h"

Sampler::Sampler(Graphics& gfx, D3D11_TEXTURE_ADDRESS_MODE mode, UINT slot)
    : slot(slot)
{
    INFOMAN(gfx);

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = mode;
    samplerDesc.AddressV = mode;
    samplerDesc.AddressW = mode;

    GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
}

void Sampler::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->PSSetSamplers(slot, 1, pSampler.GetAddressOf());
}

void Sampler::BindVS(Graphics& gfx) noexcept
{
    GetContext(gfx)->VSSetSamplers(slot, 1, pSampler.GetAddressOf());
}

void Sampler::BindCS(Graphics& gfx) noexcept
{
    GetContext(gfx)->CSSetSamplers(slot, 1, pSampler.GetAddressOf());
}

std::shared_ptr<Sampler> Sampler::Resolve(Graphics& gfx)
{
    return Codex::Resolve<Sampler>(gfx);
}
std::string Sampler::GenerateUID()
{
    return typeid(Sampler).name();
}
std::string Sampler::GetUID() const noexcept
{
    return GenerateUID();
}