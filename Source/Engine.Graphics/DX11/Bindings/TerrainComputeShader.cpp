#include "TerrainComputeShader.h"
#include "../GraphicsExceptionsMacros.h"
#include "imgui.h"

TerrainComputeShader::TerrainComputeShader(Graphics& gfx)
    :cbuf(gfx, 0u), cbData( 1024u, 1.0f, 4.0f, 2.0f, 0.5f, 8u)
{
    INFOMAN(gfx);
    terrainSampler = std::make_unique<Sampler>(gfx, D3D11_TEXTURE_ADDRESS_CLAMP);
    heightmap = std::make_unique<ComputeTexture>(gfx, HeightmapResolution, HeightmapResolution, 0u, 0u);
    normalmap = std::make_unique<ComputeTexture>(gfx, HeightmapResolution, HeightmapResolution, 1u, 1u, DXGI_FORMAT_R32G32B32A32_FLOAT);

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(std::wstring{ path.begin(),path.end() }.c_str(), &pBlob));
    GFX_THROW_INFO(GetDevice(gfx)->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComputeShader));
}


void TerrainComputeShader::Generate(Graphics& gfx)
{
    Bind(gfx);

    constexpr UINT threadGroupSize = 8u;

    const UINT groupCount =
        (HeightmapResolution + threadGroupSize - 1u) /
        threadGroupSize;

    gfx.Dispatch(
        groupCount,
        groupCount,
        1u
    );

    Unbind(gfx);
}

void TerrainComputeShader::Bind(Graphics& gfx) noexcept
{
    auto dataCopy = cbData;

    cbuf.Update(gfx, dataCopy);
    cbuf.Bind(gfx);
    GetContext(gfx)->CSSetShader(pComputeShader.Get(), nullptr, 0u);

    heightmap->BindUAV(gfx);
    normalmap->BindUAV(gfx);
}

void TerrainComputeShader::BindVS(Graphics& gfx) noexcept
{
    heightmap->BindVS(gfx);
    normalmap->BindVS(gfx);
    terrainSampler->BindVS(gfx);
}

void TerrainComputeShader::Unbind(Graphics& gfx) noexcept
{
    heightmap->UnbindUAV(gfx);
    normalmap->UnbindUAV(gfx);
    GetContext(gfx)->CSSetShader(nullptr, nullptr, 0u);
}

void TerrainComputeShader::SpawnControlWindow() noexcept
{
    if (ImGui::Begin("Terrain"))
    {
        ImGui::SliderFloat("Frequency", &cbData.frequency, 0.01f, 25.0f, "%.02f");
        ImGui::SliderFloat("Frequency Factor", &cbData.frequencyFactor, 1.0, 1.99f, "%.02f");
        ImGui::SliderFloat("Height Factor", &cbData.heightFactor, 0.01, 25.0f, "%.02f");
        ImGui::SliderFloat("Amplitude Factor", &cbData.amplitudeFactor, 0.01, 0.99f, "%.02f");

        int iterations = static_cast<int>(cbData.iterations);
        if (ImGui::SliderInt("Iterations", &iterations, 1, 128))
        {
            cbData.iterations = static_cast<UINT>(iterations);
        }

        if (ImGui::Button("Generate"))
        {
            regenCallback();
        }
    }
    ImGui::End();
}