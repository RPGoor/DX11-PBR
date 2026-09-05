#pragma once
#include "../Bindable.h"
#include "ConstantBuffers.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
  private:
    struct alignas(16) ObjectConstants
    {
        DirectX::XMMATRIX model;
        DirectX::XMMATRIX modelInverseTranspose;
    };

  public:
    TransformCbuf(Graphics& gfx, UINT slot = 1u);
    void Bind(Graphics& gfx) noexcept override;
    void SetTransform(DirectX::XMMATRIX transformMatrix);

  private:
    VertexConstantBuffer<ObjectConstants> vbuf;
    DirectX::XMFLOAT4X4 transform;
};
