#include "ComputeShader.h"
#include "../GraphicsExceptionsMacros.h"

ComputeShader::ComputeShader(Graphics& gfx, const std::string& path)
    :
    path(path), cbuf(gfx, 0u)
{
    INFOMAN(gfx);
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(std::wstring{ path.begin(),path.end() }.c_str(), &pBlob));
    GFX_THROW_INFO(GetDevice(gfx)->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComputeShader));
}

void ComputeShader::Bind(Graphics& gfx) noexcept
{
    TerrainConstants data = { 1024u };
    cbuf.Update(gfx, data);
    cbuf.Bind(gfx);
    GetContext(gfx)->CSSetShader(pComputeShader.Get(), nullptr, 0u);
}

void ComputeShader::Unbind(Graphics& gfx) noexcept
{
    GetContext(gfx)->CSSetShader(nullptr, nullptr, 0u);
}
