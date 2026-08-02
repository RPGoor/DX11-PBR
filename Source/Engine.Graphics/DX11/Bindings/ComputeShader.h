#pragma once
#include "Bindable.h"
#include "ConstantBuffers.h"
#include <functional>

class ComputeShader : public Bindable
{

public:
    ComputeShader(Graphics& gfx, const std::string& path);
    void Bind(Graphics& gfx) noexcept override;
    void Unbind(Graphics& gfx) noexcept;
    void SpawnControlWindow() noexcept;

    std::function<void()> regenCallback;

protected:
    struct alignas(16) TerrainConstants
    {
        UINT textureDimensions;
        float frequency;
        float heightFactor;
        float frequencyFactor;
        float amplitudeFactor;
        UINT iterations;
        UINT padding[2];
    };

    std::string path;
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader;

    TerrainConstants cbData;
    mutable ComputeConstantBuffer<TerrainConstants> cbuf;
};
