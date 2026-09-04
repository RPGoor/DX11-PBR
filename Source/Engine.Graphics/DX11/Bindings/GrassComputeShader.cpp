#include "GrassComputeShader.h"
#include "../GraphicsExceptionsMacros.h"
#include "imgui.h"

GrassComputeShader::GrassComputeShader(Graphics& gfx)
    : cbuf(gfx, 0u),
      cbData(0u, 20.0f, {0.0f, 0.0f})
{
    INFOMAN(gfx);
    terrainSampler = std::make_unique<Sampler>(gfx, D3D11_TEXTURE_ADDRESS_CLAMP);

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(std::wstring{path.begin(), path.end()}.c_str(), &pBlob));
    GFX_THROW_INFO(
        GetDevice(gfx)->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComputeShader)
    );
}

void GrassComputeShader::Generate(
    Graphics& gfx,
    InstanceBuffer& instanceBuffer,
    IndirectArgsBuffer& indirectArgs,
    DirectX::XMFLOAT2 chunkPosition,
    ComputeTexture& normalmap
)
{
    cbData.chunkPosition = chunkPosition;
    cbData.candidateCount = instanceBuffer.GetCount();
    Bind(gfx);
    normalmap.BindCS(gfx);
    terrainSampler->BindCS(gfx);

    ID3D11UnorderedAccessView* uav = instanceBuffer.GetUAV();
    UINT initialCount = 0u;

    GetContext(gfx)->CSSetUnorderedAccessViews(0u, 1u, &uav, &initialCount);
    const UINT groupCount = (cbData.candidateCount + 256u - 1u) / 256u;
    gfx.Dispatch(groupCount, 1u, 1u);

    GetContext(gfx)->CopyStructureCount(indirectArgs.GetBuffer(), sizeof(UINT), uav);

    Unbind(gfx);
}

void GrassComputeShader::Bind(Graphics& gfx) noexcept
{
    auto dataCopy = cbData;
    cbuf.Update(gfx, dataCopy);
    cbuf.Bind(gfx);
    GetContext(gfx)->CSSetShader(pComputeShader.Get(), nullptr, 0u);
}

void GrassComputeShader::Unbind(Graphics& gfx) noexcept
{
    ID3D11UnorderedAccessView* nullUAV = nullptr;
    GetContext(gfx)->CSSetUnorderedAccessViews(0u, 1u, &nullUAV, nullptr);
    GetContext(gfx)->CSSetShader(nullptr, nullptr, 0u);
}
