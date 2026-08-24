#pragma once
#include "Bindable.h"
#include "../../Core/Vertex.h"

class InputLayout : public Bindable
{
public:
    InputLayout(Graphics& gfx,
        Dvtx::VertexLayout layout,
        ID3DBlob* pVertexShaderBytecode);

    InputLayout(
        Graphics& gfx,
        const Dvtx::VertexLayout& vertexLayout,
        const Dvtx::VertexLayout& instanceLayout,
        ID3DBlob* pVertexShaderBytecode
    );

    void Bind(Graphics& gfx) noexcept override;

    static std::shared_ptr<InputLayout> Resolve(
        Graphics& gfx,
        const Dvtx::VertexLayout& layout,
        ID3DBlob* pVertexShaderBytecode);

    static std::shared_ptr<InputLayout> ResolveInstanced(
        Graphics& gfx,
        const Dvtx::VertexLayout& vertexLayout,
        const Dvtx::VertexLayout& instanceLayout,
        ID3DBlob* pVertexShaderBytecode
    );

    static std::string GenerateUID(const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode = nullptr);
    static std::string GenerateUID(
        const Dvtx::VertexLayout& vertexLayout,
        const Dvtx::VertexLayout& instanceLayout,
        ID3DBlob* pVertexShaderBytecode = nullptr
    );
    std::string GetUID() const noexcept override;
protected:
    Dvtx::VertexLayout layout;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};
