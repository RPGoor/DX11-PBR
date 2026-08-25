#pragma once
#include "Bindable.h"
#include "ConstantBuffers.h"
#include <functional>
#include "Sampler.h"
#include "ComputeTexture.h"

class TerrainComputeShader : public Bindable
{

public:
    TerrainComputeShader(Graphics& gfx);
    void Generate(Graphics& gfx);
    void Bind(Graphics& gfx) noexcept override;
    void BindVS(Graphics& gfx) noexcept;

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

    std::string path = "TerrainCS.cso";
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader;

    TerrainConstants cbData;
    mutable ComputeConstantBuffer<TerrainConstants> cbuf;

    static constexpr UINT HeightmapResolution = 1024u;
    std::unique_ptr<ComputeTexture> heightmap;
    std::unique_ptr<ComputeTexture> normalmap;
    std::unique_ptr<Sampler> terrainSampler;
};
