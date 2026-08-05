#include "ShadowMap.h"
#include "../GraphicsExceptionsMacros.h"

ShadowMap::ShadowMap(Graphics& gfx, UINT slot)
    : slot(slot), cbuf(gfx, 4u)
{
    INFOMAN(gfx);

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1u;
    textureDesc.ArraySize = 1u;
    textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    textureDesc.SampleDesc.Count = 1u;
    textureDesc.SampleDesc.Quality = 0u;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags =
        D3D11_BIND_DEPTH_STENCIL |
        D3D11_BIND_SHADER_RESOURCE;

    GFX_THROW_INFO(
        GetDevice(gfx)->CreateTexture2D(
            &textureDesc,
            nullptr,
            &texture
        )
    );

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0u;

    GFX_THROW_INFO(
        GetDevice(gfx)->CreateDepthStencilView(
            texture.Get(),
            &dsvDesc,
            &depthView
        )
    );

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0u;
    srvDesc.Texture2D.MipLevels = 1u;

    GFX_THROW_INFO(
        GetDevice(gfx)->CreateShaderResourceView(
            texture.Get(),
            &srvDesc,
            &shaderView
        )
    );
}

void ShadowMap::BindAsDepthBuffer(Graphics& gfx)
{
    ID3D11ShaderResourceView* nullSrv = nullptr;

    GetContext(gfx)->PSSetShaderResources(2u, 1u, &nullSrv);
    GetContext(gfx)->PSSetShader(nullptr, nullptr, 0u);

    GetContext(gfx)->OMSetRenderTargets(
        0u,
        nullptr,
        depthView.Get()
    );

    GetContext(gfx)->ClearDepthStencilView(
        depthView.Get(),
        D3D11_CLEAR_DEPTH,
        1.0f,
        0u
    );

    gfx.CreateViewport();

    auto dataCopy = cbData;
    cbuf.Update(gfx, dataCopy);
    cbuf.Bind(gfx);
}

void ShadowMap::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->PSSetShaderResources(
        slot,
        1u,
        shaderView.GetAddressOf()
    );

    auto dataCopy = cbData;
    cbuf.Update(gfx, dataCopy);
    cbuf.Bind(gfx);
}

void ShadowMap::SetLightViewProjection(Graphics& gfx, const DirectX::XMFLOAT3& lightDirection, const DirectX::XMFLOAT3& cameraPosition)
{
    using namespace DirectX;

    const XMVECTOR direction = XMVector3Normalize(
        XMLoadFloat3(&lightDirection)
    );

    const XMVECTOR target = XMVectorSet(
        cameraPosition.x,
        0.0f,
        cameraPosition.z,
        1.0f
    );

    const XMVECTOR eye = target - direction * 50.0f;

    XMVECTOR up = XMVectorSet(0, 1, 0, 0);

    if (std::abs(XMVectorGetY(direction)) > 0.99f)
    {
        up = XMVectorSet(0, 0, 1, 0);
    }

    const XMMATRIX view = XMMatrixLookAtLH(
        eye,
        target,
        up
    );

    const XMMATRIX projection = XMMatrixOrthographicLH(
        80.0f,   // width
        80.0f,   // height
        0.1f,    // near
        150.0f   // far
    );

    XMStoreFloat4x4(
        &cbData.lightViewProjection,
        view * projection
    );
}


