#include "InputLayout.h"
#include "../GraphicsExceptionsMacros.h"
#include "BindableCodex.h"

InputLayout::InputLayout(Graphics& gfx,
    Dvtx::VertexLayout layout_in,
    ID3DBlob* pVertexShaderBytecode)
    :
    layout(std::move(layout_in))
{
    INFOMAN(gfx);

    const auto d3dLayout = layout.GetD3DLayout();

    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
        d3dLayout.data(), (UINT)d3dLayout.size(),
        pVertexShaderBytecode->GetBufferPointer(),
        pVertexShaderBytecode->GetBufferSize(),
        &pInputLayout
    ));
}

InputLayout::InputLayout(
    Graphics& gfx,
    const Dvtx::VertexLayout& vertexLayout,
    const Dvtx::VertexLayout& instanceLayout,
    ID3DBlob* pVertexShaderBytecode
)
{
    INFOMAN(gfx);

    auto vertexDescs = vertexLayout.GetD3DLayout();
    auto instanceDescs = instanceLayout.GetD3DLayout();

    std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
    descs.reserve(
        vertexDescs.size() +
        instanceDescs.size()
    );

    descs.insert(
        descs.end(),
        vertexDescs.begin(),
        vertexDescs.end()
    );

    descs.insert(
        descs.end(),
        instanceDescs.begin(),
        instanceDescs.end()
    );

    GFX_THROW_INFO(
        GetDevice(gfx)->CreateInputLayout(
            descs.data(),
            static_cast<UINT>(descs.size()),
            pVertexShaderBytecode->GetBufferPointer(),
            pVertexShaderBytecode->GetBufferSize(),
            &pInputLayout
        )
    );
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
std::shared_ptr<InputLayout> InputLayout::Resolve(Graphics& gfx,
    const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
{
    return Codex::Resolve<InputLayout>(gfx, layout, pVertexShaderBytecode);
}

std::shared_ptr<InputLayout> InputLayout::ResolveInstanced(Graphics& gfx, const Dvtx::VertexLayout& vertexLayout, const Dvtx::VertexLayout& instanceLayout, ID3DBlob* pVertexShaderBytecode)
{
    return Codex::Resolve<InputLayout>(gfx, vertexLayout, instanceLayout, pVertexShaderBytecode);
}



std::string InputLayout::GenerateUID(const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
{
    using namespace std::string_literals;
    return typeid(InputLayout).name() + "#"s + layout.GetCode();
}
std::string InputLayout::GenerateUID(const Dvtx::VertexLayout& vertexLayout, const Dvtx::VertexLayout& instanceLayout, ID3DBlob* pVertexShaderBytecode)
{
    using namespace std::string_literals;

    return typeid(InputLayout).name()
        + "#"s
        + vertexLayout.GetCode()
        + "#"
        + instanceLayout.GetCode();
}
std::string InputLayout::GetUID() const noexcept
{
    return GenerateUID(layout);
}