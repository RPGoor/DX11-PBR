#include "InstanceBuffer.h"
#include "../GraphicsExceptionsMacros.h"

InstanceBuffer::InstanceBuffer(Graphics& gfx, const std::vector<Vertex::Instance>& instances)
    : count(static_cast<UINT>(instances.size()))
{
    INFOMAN(gfx);

    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = static_cast<UINT>(sizeof(Vertex::Instance) * instances.size());
    bd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = instances.data();

    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(
        &bd,
        &sd,
        &pBuffer
    ));
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
