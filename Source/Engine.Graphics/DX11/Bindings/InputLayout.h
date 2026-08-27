#pragma once
#include "../Bindable.h"
#include "../../Core/Vertex.h"
#include <span>

class InputLayout : public Bindable
{
public:
    InputLayout(Graphics& gfx,
        std::span<const D3D11_INPUT_ELEMENT_DESC> layout,
        ID3DBlob* pVertexShaderBytecode);

    void Bind(Graphics& gfx) noexcept override;

    static std::shared_ptr<InputLayout> Resolve(
        Graphics& gfx,
        std::span<const D3D11_INPUT_ELEMENT_DESC> layout,
        ID3DBlob* pVertexShaderBytecode);

    static std::string GenerateUID(std::span<const D3D11_INPUT_ELEMENT_DESC> layout, ID3DBlob* pVertexShaderBytecode = nullptr);

    std::string GetUID() const noexcept override;
protected:
    std::span<const D3D11_INPUT_ELEMENT_DESC> layout;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};
