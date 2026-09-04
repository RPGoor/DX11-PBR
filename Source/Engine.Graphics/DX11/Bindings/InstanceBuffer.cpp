#include "../GraphicsExceptionsMacros.h"
#include "InstanceBuffer.h"

InstanceBuffer::InstanceBuffer(Graphics& gfx, UINT maxCount)
    : count(maxCount)
{
    INFOMAN(gfx);

    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bd.ByteWidth = static_cast<UINT>(sizeof(Vertex::Instance) * maxCount);
    bd.StructureByteStride = stride;

    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, nullptr, &pBuffer));

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.FirstElement = 0u;
    uavDesc.Buffer.NumElements = maxCount;
    uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
    GFX_THROW_INFO(GetDevice(gfx)->CreateUnorderedAccessView(pBuffer.Get(), &uavDesc, &pUnorderedAccessView));

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0u;
    srvDesc.Buffer.NumElements = maxCount;
    GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pBuffer.Get(), &srvDesc, &pShaderResourceView));
}

void InstanceBuffer::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->VSSetShaderResources(2u, 1u, pShaderResourceView.GetAddressOf());
}

UINT InstanceBuffer::GetCount() const noexcept
{
    return count;
}

ID3D11UnorderedAccessView* InstanceBuffer::GetUAV() const noexcept
{
    return pUnorderedAccessView.Get();
}
