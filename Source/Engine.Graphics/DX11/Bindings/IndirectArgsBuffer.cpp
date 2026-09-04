#include "IndirectArgsBuffer.h"
#include "../GraphicsExceptionsMacros.h"

IndirectArgsBuffer::IndirectArgsBuffer(Graphics& gfx, UINT indexCount)
{
    INFOMAN(gfx);

    D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS args = {};

    args.IndexCountPerInstance = indexCount;
    args.InstanceCount = 0u;
    args.StartIndexLocation = 0u;
    args.BaseVertexLocation = 0;
    args.StartInstanceLocation = 0u;

    D3D11_BUFFER_DESC desc = {};
    desc.BindFlags = 0u;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.CPUAccessFlags = 0u;
    desc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
    desc.ByteWidth = sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS);
    desc.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = &args;
    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&desc, &data, &indirectArgs));
}

ID3D11Buffer* IndirectArgsBuffer::GetBuffer() const noexcept
{
    return indirectArgs.Get();
}
