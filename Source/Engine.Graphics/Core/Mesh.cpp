#include "Mesh.h"
#include "../DX11/Bindings/Topology.h"
#include "../DX11/Bindings/TransformCbuf.h"
#include "../DX11/Bindings/VertexBuffer.h"

Mesh::Mesh(Graphics& gfx, MeshData data)
{
    AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    AddBind(VertexBuffer::Resolve(gfx, data.tag, data.vertices));
    AddBind(IndexBuffer::Resolve(gfx, data.tag, data.indices));

    AddBind(std::make_shared<TransformCbuf>(gfx, *this, 1u));
}

void Mesh::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const conexcept
{
    DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
    Drawable::Draw(gfx);
}

void Mesh::DrawInstanced(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform, UINT instanceCount) const conexcept
{
    DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);

    Drawable::DrawInstanced(gfx, instanceCount);
}


DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
    return DirectX::XMLoadFloat4x4(&transform);
}
