#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, UINT slot)
    : vbuf(gfx, slot)
{
    DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixIdentity());
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
    const DirectX::XMMATRIX model = DirectX::XMLoadFloat4x4(&transform);;

    const DirectX::XMMATRIX inverseModel = DirectX::XMMatrixInverse(nullptr, model);

    const ObjectConstants tf =
    {
        model,
        DirectX::XMMatrixTranspose(inverseModel)
    };

    vbuf.Update(gfx, tf);
    vbuf.Bind(gfx);
}
