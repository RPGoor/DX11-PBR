#include "InstanceBuffer.h"

InstanceBuffer::InstanceBuffer(
    Graphics& gfx,
    const std::vector<InstanceData>& instances
)
    : count(static_cast<UINT>(instances.size()))
{
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = static_cast<UINT>(sizeof(InstanceData) * instances.size());
    bd.StructureByteStride = sizeof(InstanceData);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = instances.data();

    GetDevice(gfx)->CreateBuffer(
        &bd,
        &sd,
        &pBuffer
    );
}

void InstanceBuffer::Bind(Graphics& gfx) noexcept
{
    const UINT offset = 0u;

    GetContext(gfx)->IASetVertexBuffers(
        1u,
        1u,
        pBuffer.GetAddressOf(),
        &stride,
        &offset
    );
}

UINT InstanceBuffer::GetCount() const noexcept
{
    return count;
}
