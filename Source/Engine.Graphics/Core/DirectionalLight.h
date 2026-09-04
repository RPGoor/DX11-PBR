#pragma once
#include "../DX11/Graphics.h"
#include "../DX11/Bindings/ConstantBuffers.h"

class DirectionalLight
{
public:
    DirectionalLight( Graphics& gfx,float radius = 0.5f );

    void Bind( Graphics& gfx,DirectX::FXMMATRIX view ) const noexcept;
private:
    struct alignas(16) DirectionalLightConstants
    {
        DirectX::XMFLOAT3 directionWS;
        float intensity;

        DirectX::XMFLOAT3 color;
        float padding;
    };

private:
    DirectionalLightConstants cbData;
    mutable PixelConstantBuffer<DirectionalLightConstants> cbuf;
};