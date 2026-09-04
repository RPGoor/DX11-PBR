#pragma once
#include "../../Core/Vertex.h"
#include "../Bindable.h"
#include "../Graphics.h"
#include <span>

class PipelineSettings : Bindable
{
  public:
    PipelineSettings(
        Graphics& gfx,
        std::span<const D3D11_INPUT_ELEMENT_DESC> layout,
        const std::string& vertexShaderPath,
        const std::string& pixelShaderPath
    );

    void Bind(Graphics& gfx) noexcept override;

  private:
    std::vector<std::shared_ptr<Bindable>> bindables;
};
