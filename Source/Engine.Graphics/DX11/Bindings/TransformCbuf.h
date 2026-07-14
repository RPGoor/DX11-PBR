#pragma once
#include "Bindable.h"
#include "../../Core/Drawable.h"
#include "ConstantBuffers.h"

class TransformCbuf : public Bindable
{
private:
    struct alignas(16) ObjectConstants
    {
        DirectX::XMMATRIX model;
        DirectX::XMMATRIX modelInverseTranspose;
    };
public:
    TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot = 1u);
    void Bind(Graphics& gfx) noexcept override;
private:
    static std::unique_ptr<VertexConstantBuffer<ObjectConstants>> pVcbuf;
    const Drawable& parent;
};