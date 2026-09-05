#include "TerrainComputeShader.h"
#include "../GraphicsExceptionsMacros.h"
#include "imgui.h"
#include <d3dcompiler.h>

TerrainComputeShader::TerrainComputeShader(Graphics& gfx)
    : cbuf(gfx, 0u),
      cbData(512u, 1.0f, 4.0f, 2.0f, 0.5f, 8u, {}, 0u, {})
{
    INFOMAN(gfx);
    terrainSampler = std::make_unique<Sampler>(gfx, D3D11_TEXTURE_ADDRESS_CLAMP);

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(std::wstring{path.begin(), path.end()}.c_str(), &pBlob));
    GFX_THROW_INFO(
        GetDevice(gfx)->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComputeShader)
    );
}

void TerrainComputeShader::Generate(
    Graphics& gfx,
    ComputeTexture& heightmap,
    ComputeTexture& normalmap,
    DirectX::XMFLOAT2 position
)
{
    cbData.chunkPosition = position;
    heightmap.BindUAV(gfx);
    normalmap.BindUAV(gfx);
    Bind(gfx);

    constexpr UINT threadGroupSize = 8u;

    const UINT groupCount = (HeightmapResolution + threadGroupSize - 1u) / threadGroupSize;

    gfx.Dispatch(groupCount, groupCount, 1u);

    heightmap.UnbindUAV(gfx);
    normalmap.UnbindUAV(gfx);
    Unbind(gfx);
}

void TerrainComputeShader::Bind(Graphics& gfx) noexcept
{
    auto dataCopy = cbData;

    cbuf.Update(gfx, dataCopy);
    cbuf.Bind(gfx);
    GetContext(gfx)->CSSetShader(pComputeShader.Get(), nullptr, 0u);
}

void TerrainComputeShader::BindVS(Graphics& gfx) noexcept
{
    terrainSampler->BindVS(gfx);
}

void TerrainComputeShader::Unbind(Graphics& gfx) noexcept
{
    GetContext(gfx)->CSSetShader(nullptr, nullptr, 0u);
}

void TerrainComputeShader::SpawnControlWindow() noexcept
{
    if (ImGui::Begin("Terrain"))
    {
        ImGui::SliderFloat("Frequency", &cbData.frequency, 0.01f, 25.0f, "%.02f");
        ImGui::SliderFloat("Frequency Factor", &cbData.frequencyFactor, 1.0f, 1.99f, "%.02f");
        ImGui::SliderFloat("Height Factor", &cbData.heightFactor, 0.01f, 25.0f, "%.02f");
        ImGui::SliderFloat("Amplitude Factor", &cbData.amplitudeFactor, 0.01f, 0.99f, "%.02f");

        int iterations = static_cast<int>(cbData.iterations);
        if (ImGui::SliderInt("Iterations", &iterations, 1, 128))
        {
            cbData.iterations = static_cast<UINT>(iterations);
        }

        int seed = static_cast<int>(cbData.seed);
        if (ImGui::SliderInt("Seed", &seed, 0, 1024))
        {
            cbData.seed = static_cast<UINT>(seed);
        }

        if (ImGui::Button("Generate"))
        {
            regenCallback();
        }
    }
    ImGui::End();
}
