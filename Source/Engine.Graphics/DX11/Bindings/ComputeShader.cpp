#include "ComputeShader.h"
#include "../GraphicsExceptionsMacros.h"
#include "imgui.h"

ComputeShader::ComputeShader(Graphics& gfx, const std::string& path)
    :
    path(path), cbuf(gfx, 0u), cbData( 1024u, 1.0f, 1.0f, 2.0f, 0.5f, 8u)
{
    INFOMAN(gfx);
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(std::wstring{ path.begin(),path.end() }.c_str(), &pBlob));
    GFX_THROW_INFO(GetDevice(gfx)->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComputeShader));
}

void ComputeShader::Bind(Graphics& gfx) noexcept
{
    auto dataCopy = cbData;

    cbuf.Update(gfx, dataCopy);
    cbuf.Bind(gfx);
    GetContext(gfx)->CSSetShader(pComputeShader.Get(), nullptr, 0u);
}

void ComputeShader::Unbind(Graphics& gfx) noexcept
{
    GetContext(gfx)->CSSetShader(nullptr, nullptr, 0u);
}

void ComputeShader::SpawnControlWindow() noexcept
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