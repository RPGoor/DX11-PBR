#include "ComputeTexture.h"
#include "../GraphicsExceptionsMacros.h"

ComputeTexture::ComputeTexture(
    Graphics& gfx,
    UINT width,
    UINT height,
    UINT shaderResourceSlot,
    UINT unorderedAccessSlot
)
    :
    width(width),
    height(height),
    shaderResourceSlot(shaderResourceSlot),
    unorderedAccessSlot(unorderedAccessSlot)
{
    INFOMAN(gfx);

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1u;
    textureDesc.ArraySize = 1u;
    textureDesc.Format = format;
    textureDesc.SampleDesc.Count = 1u;
    textureDesc.SampleDesc.Quality = 0u;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;

    textureDesc.BindFlags =
        D3D11_BIND_SHADER_RESOURCE |
        D3D11_BIND_UNORDERED_ACCESS;

    textureDesc.CPUAccessFlags = 0u;
    textureDesc.MiscFlags = 0u;

    GFX_THROW_INFO(
        GetDevice(gfx)->CreateTexture2D(
            &textureDesc,
            nullptr,
            &pTexture
        )
    );

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0u;
    srvDesc.Texture2D.MipLevels = 1u;

    GFX_THROW_INFO(
        GetDevice(gfx)->CreateShaderResourceView(
            pTexture.Get(),
            &srvDesc,
            &pTextureView
        )
    );

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = format;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    uavDesc.Texture2D.MipSlice = 0u;

    GFX_THROW_INFO(
        GetDevice(gfx)->CreateUnorderedAccessView(
            pTexture.Get(),
            &uavDesc,
            &pUnorderedAccessView
        )
    );
}

void ComputeTexture::Bind(Graphics& gfx) noexcept
{
    BindVS(gfx);
}

void ComputeTexture::BindVS(Graphics& gfx) noexcept
{
    GetContext(gfx)->VSSetShaderResources(
        shaderResourceSlot,
        1u,
        pTextureView.GetAddressOf()
    );
}

void ComputeTexture::BindPS(Graphics& gfx) noexcept
{
    GetContext(gfx)->PSSetShaderResources(
        shaderResourceSlot,
        1u,
        pTextureView.GetAddressOf()
    );
}

void ComputeTexture::BindCS(Graphics& gfx) noexcept
{
    GetContext(gfx)->CSSetShaderResources(
        shaderResourceSlot,
        1u,
        pTextureView.GetAddressOf()
    );
}

void ComputeTexture::BindUAV(Graphics& gfx) noexcept
{
    ID3D11UnorderedAccessView* const uav =
        pUnorderedAccessView.Get();

    GetContext(gfx)->CSSetUnorderedAccessViews(
        unorderedAccessSlot,
        1u,
        &uav,
        nullptr
    );
}


void ComputeTexture::UnbindCSResource(Graphics& gfx) noexcept
{
    ID3D11ShaderResourceView* nullSRV = nullptr;

    GetContext(gfx)->CSSetShaderResources(
        shaderResourceSlot,
        1u,
        &nullSRV
    );
}

void ComputeTexture::UnbindUAV(Graphics& gfx) noexcept
{
    ID3D11UnorderedAccessView* nullUAV = nullptr;

    GetContext(gfx)->CSSetUnorderedAccessViews(
        unorderedAccessSlot,
        1u,
        &nullUAV,
        nullptr
    );
}

UINT ComputeTexture::GetWidth() const noexcept
{
    return width;
}

UINT ComputeTexture::GetHeight() const noexcept
{
    return height;
}

ID3D11ShaderResourceView* ComputeTexture::GetSRV() const noexcept
{
    return pTextureView.Get();
}

ID3D11UnorderedAccessView* ComputeTexture::GetUAV() const noexcept
{
    return pUnorderedAccessView.Get();
}