#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
    Sampler(Graphics& gfx);
    void Bind(Graphics& gfx) noexcept override;
    void BindVS(Graphics& gfx) noexcept ;

    static std::shared_ptr<Sampler> Resolve(Graphics& gfx);
    static std::string GenerateUID();
    std::string GetUID() const noexcept override;
protected:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};