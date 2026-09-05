#pragma once
#include "../Bindable.h"
#include "ComputeTexture.h"
#include "ConstantBuffers.h"
#include "IndirectArgsBuffer.h"
#include "InstanceBuffer.h"
#include "Sampler.h"
#include <functional>

class GrassComputeShader : public Bindable
{
  public:
    GrassComputeShader(Graphics& gfx);

  public:
    void Generate(
        Graphics& gfx,
        InstanceBuffer& instanceBuffer,
        IndirectArgsBuffer& indirectArgs,
        DirectX::XMFLOAT2 chunkPosition,
        ComputeTexture& normalmap
    );
    void Bind(Graphics& gfx) noexcept override;
    void Unbind(Graphics& gfx) noexcept;

    std::function<void()> regenCallback;

  protected:
    struct alignas(16) GrassGenerationConstants
    {
        UINT candidateCount;
        float chunkSize;
        DirectX::XMFLOAT2 chunkPosition;
    };

    std::string path = "GrassCS.cso";
    std::unique_ptr<Sampler> terrainSampler;
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader;
    GrassGenerationConstants cbData;
    mutable ComputeConstantBuffer<GrassGenerationConstants> cbuf;
};
