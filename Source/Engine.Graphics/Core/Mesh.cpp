#include "Mesh.h"
#include "../DX11/Bindings/Topology.h"
#include "../DX11/Bindings/TransformCbuf.h"

Mesh::Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bindable>> bindPtrs)
{
    AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    for (auto& pb : bindPtrs)
    {
        AddBind(std::move(pb));
    }

    AddBind(std::make_shared<TransformCbuf>(gfx, *this, 1u));
}

void Mesh::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const conexcept
{
    DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
    Drawable::Draw(gfx);
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
    return DirectX::XMLoadFloat4x4(&transform);
}
