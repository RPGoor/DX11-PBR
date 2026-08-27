#pragma once

#include "../Bindable.h"

class ComputeTexture : public Bindable
{
public:
    ComputeTexture(
        Graphics& gfx,
        UINT width,
        UINT height,
        UINT shaderResourceSlot = 0u,
        UINT unorderedAccessSlot = 0u,
        DXGI_FORMAT format = DXGI_FORMAT_R32_FLOAT
    );

    void Bind(Graphics& gfx) noexcept override;

    void BindVS(Graphics& gfx) noexcept;
    void BindPS(Graphics& gfx) noexcept;
    void BindCS(Graphics& gfx) noexcept;
    void BindUAV(Graphics& gfx) noexcept;

    void UnbindCSResource(Graphics& gfx) noexcept;
    void UnbindUAV(Graphics& gfx) noexcept;

    UINT GetWidth() const noexcept;
    UINT GetHeight() const noexcept;

    ID3D11ShaderResourceView* GetSRV() const noexcept;
    ID3D11UnorderedAccessView* GetUAV() const noexcept;

private:
    UINT width;
    UINT height;
    UINT shaderResourceSlot;
    UINT unorderedAccessSlot;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> pUnorderedAccessView;
};