#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
    :
    parent(parent)
{
    if (!pVcbuf)
    {
        pVcbuf = std::make_unique<VertexConstantBuffer<ObjectConstants>>(gfx, slot);
    }
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
    const DirectX::XMMATRIX model =
        parent.GetTransformXM();

    const DirectX::XMMATRIX inverseModel =
        DirectX::XMMatrixInverse(nullptr, model);

    const ObjectConstants tf =
    {
        model,
        DirectX::XMMatrixTranspose(inverseModel)
    };

    pVcbuf->Update(gfx, tf);
    pVcbuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCbuf::ObjectConstants>> TransformCbuf::pVcbuf;