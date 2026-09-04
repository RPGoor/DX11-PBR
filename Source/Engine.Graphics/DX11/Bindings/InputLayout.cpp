#include "../BindableCodex.h"
#include "../GraphicsExceptionsMacros.h"
#include "InputLayout.h"

InputLayout::InputLayout(
    Graphics& gfx,
    std::span<const D3D11_INPUT_ELEMENT_DESC> layout,
    ID3DBlob* pVertexShaderBytecode
)
    : layout(std::move(layout))
{
    INFOMAN(gfx);

    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
        layout.data(),
        (UINT)layout.size(),
        pVertexShaderBytecode->GetBufferPointer(),
        pVertexShaderBytecode->GetBufferSize(),
        &pInputLayout
    ));
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}

std::shared_ptr<InputLayout>
InputLayout::Resolve(Graphics& gfx, std::span<const D3D11_INPUT_ELEMENT_DESC> layout, ID3DBlob* pVertexShaderBytecode)
{
    return Codex::Resolve<InputLayout>(gfx, layout, pVertexShaderBytecode);
}

std::string InputLayout::GenerateUID(std::span<const D3D11_INPUT_ELEMENT_DESC> layout, ID3DBlob* pVertexShaderBytecode)
{
    using namespace std::string_literals;
    return typeid(InputLayout).name() + "#"s + std::to_string(layout.size());
}

std::string InputLayout::GetUID() const noexcept
{
    return GenerateUID(layout);
}
