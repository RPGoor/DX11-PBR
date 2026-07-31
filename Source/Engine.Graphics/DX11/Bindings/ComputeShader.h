#pragma once
#include "Bindable.h"
#include "ConstantBuffers.h"

class ComputeShader : public Bindable
{

public:
    ComputeShader(Graphics& gfx, const std::string& path);
    void Bind(Graphics& gfx) noexcept override;
    void Unbind(Graphics& gfx) noexcept;
protected:
    struct alignas(16) TerrainConstants
    {
        UINT textureDimensions;
        UINT padding[3];
    };

    std::string path;
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader;
    mutable ComputeConstantBuffer<TerrainConstants> cbuf;
};
