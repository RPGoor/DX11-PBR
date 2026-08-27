#include "PipelineSettings.h"
#include "../DX11/Bindings/VertexShader.h"
#include "../DX11/Bindings/PixelShader.h"
#include "../DX11/Bindings/InputLayout.h"

PipelineSettings::PipelineSettings(
    Graphics& gfx,
    std::span<const D3D11_INPUT_ELEMENT_DESC> layout,
    const std::string& vertexShaderPath,
    const std::string& pixelShaderPath
)
{
    auto pvs = VertexShader::Resolve(gfx, vertexShaderPath);

    auto pvsBytecode = pvs->GetBytecode();

    bindables.push_back(pvs);

    bindables.push_back(PixelShader::Resolve(gfx, pixelShaderPath));

    bindables.push_back(InputLayout::Resolve(gfx, layout, pvsBytecode));
}

void PipelineSettings::Bind(Graphics& gfx) noexcept
{
    for (const auto& bindable : bindables)
    {
        bindable->Bind(gfx);
    }
}