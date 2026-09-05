#pragma once
#include "../Bindable.h"
#include "../Graphics.h"
#include <d3d11.h>
#include <memory>
#include <span>
#include <vector>

class PipelineSettings : public Bindable
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
