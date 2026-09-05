#pragma once
#include "../Bindable.h"
#include "ConstantBuffers.h"
#include <DirectXMath.h>

class FrameCbuf : public Bindable
{
  public:
    FrameCbuf(Graphics& gfx);

  public:
    void Update(
        Graphics& gfx,
        DirectX::FXMMATRIX view,
        DirectX::FXMMATRIX projection,
        const DirectX::XMFLOAT3& cameraPosition,
        float time
    );
    void Bind(Graphics& gfx) noexcept override;

  private:
    struct alignas(16) FrameConstants
    {
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
        DirectX::XMMATRIX viewProjection;
        DirectX::XMFLOAT3 cameraPositionWS;
        float time;
    };

    VertexConstantBuffer<FrameConstants> bufferVS;
    PixelConstantBuffer<FrameConstants> bufferPS;
};
