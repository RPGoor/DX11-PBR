#pragma once
#include "../Bindable.h"
#include "ConstantBuffers.h"
#include <functional>
#include "ComputeTexture.h"
#include "IndirectArgsBuffer.h"
#include "InstanceBuffer.h"

class GrassComputeShader : public Bindable
{

public:
    GrassComputeShader(Graphics& gfx);
    void Generate(Graphics& gfx, InstanceBuffer& instanceBuffer, IndirectArgsBuffer& indirectArgs, DirectX::XMFLOAT2 chunkPosition);
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
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader;

    GrassGenerationConstants cbData;
    mutable ComputeConstantBuffer<GrassGenerationConstants> cbuf;
};
