#pragma once
#include "../Graphics.h"
#include "Bindable.h"
#include "ConstantBuffers.h"

class ShadowMap : public Bindable
{
public:
    ShadowMap(Graphics& gfx, UINT slot = 0u);
    void BindAsDepthBuffer(Graphics& gfx);
    void Bind(Graphics& gfx) noexcept override;

    void SetLightViewProjection(Graphics& gfx, const DirectX::XMFLOAT3& lightDirection, const DirectX::XMFLOAT3& cameraPosition);

private:
    UINT width = 2048u;
    UINT height = 2048u;
    UINT slot;

    struct alignas(16) ShadowTransforms
    {
        DirectX::XMFLOAT4X4 lightViewProjection;
    };

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthView;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderView;

    ShadowTransforms cbData;
    VertexConstantBuffer<ShadowTransforms> cbuf;
};