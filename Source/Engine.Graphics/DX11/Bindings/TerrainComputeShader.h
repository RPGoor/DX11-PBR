#pragma once
#include "../Bindable.h"
#include "ConstantBuffers.h"
#include <functional>
#include "Sampler.h"
#include "ComputeTexture.h"

class TerrainComputeShader : public Bindable
{

public:
    TerrainComputeShader(Graphics& gfx);
    void Generate(Graphics& gfx, ComputeTexture& heightmap, ComputeTexture& normalmap, DirectX::XMFLOAT2 position);
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
        DirectX::XMFLOAT2 chunkPosition;
        UINT seed;
        UINT padding[3];
    };

    std::string path = "TerrainCS.cso";
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader;

    TerrainConstants cbData;
    mutable ComputeConstantBuffer<TerrainConstants> cbuf;

    static constexpr UINT HeightmapResolution = 512u;
    std::unique_ptr<Sampler> terrainSampler;
};
