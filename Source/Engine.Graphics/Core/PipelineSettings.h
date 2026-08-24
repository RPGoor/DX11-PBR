#pragma once
#include "../DX11/Graphics.h"
#include "Vertex.h"
#include "../DX11/Bindings/Bindable.h"

class PipelineSettings : Bindable
{
public:
    PipelineSettings(
        Graphics& gfx,
        const Dvtx::VertexLayout& layout,
        const std::string& vertexShaderPath,
        const std::string& pixelShaderPath
    );

    PipelineSettings(
        Graphics& gfx,
        const Dvtx::VertexLayout& vertexLayout,
        const Dvtx::VertexLayout& instanceLayout,
        const std::string& vertexShaderPath,
        const std::string& pixelShaderPath
    );


    void Bind(Graphics& gfx) noexcept override;

private:
    std::vector<std::shared_ptr<Bindable>> bindables;
};