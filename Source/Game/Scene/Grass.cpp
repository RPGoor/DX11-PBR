#include "Grass.h"
#include <random>
#include "ImGui.h"
#include <MeshFactory.h>

Grass::Grass(Graphics& gfx)
    : cbuf(gfx, 2u), cbData(1.0f, 1.5f, 5.0f, 0.0f, { 1.0f, 1.0f }, 8.0f, 0.0f)
{
    noiseTexture = std::make_unique<Texture>(
        gfx,
        "..\\..\\Assets\\Textures\\noise2.png",
        1u
    );

    MeshData data = MeshFactory::Load("..\\..\\Assets\\Models\\grass.obj");
    pipeline = std::make_unique<PipelineSettings>(
        gfx,
        Vertex::CombineLayouts(VertexLayout<Vertex::Standard>::elements, VertexLayout<Vertex::Instance>::elements),
        "GrassVS.cso",
        "GrassPS.cso");
    material = Material::Resolve(gfx, "M_grass", MaterialConstants{{0.15f, 0.63f, 0.23f}, 0.0, 0.32, {}});
    mesh = Mesh::Resolve(gfx, data);
    transform = std::make_unique<TransformCbuf>(gfx);

    noiseSampler = std::make_unique<Sampler>(gfx, D3D11_TEXTURE_ADDRESS_WRAP, 1u);
}

void Grass::Bind(Graphics& gfx) const
{
    auto dataCopy = cbData;
    cbuf.Update(gfx, dataCopy);

    cbuf.Bind(gfx);
    noiseTexture->BindVS(gfx);
    noiseSampler->BindVS(gfx);
}

void Grass::SpawnControlWindow() noexcept
{
    if (ImGui::Begin("Grass"))
    {
        ImGui::SliderFloat("Bend Strength", &cbData.bendStrength, 0.0, 2.0f, "%.02f");
        ImGui::SliderFloat("Mask Power", &cbData.bendMaskPow, 1.0, 4.0f, "%.02f");
        ImGui::SliderFloat("Speed", &cbData.speed, 0.01, 25.0f, "%.02f");
        ImGui::SliderFloat2("Direction", cbData.direction, -1.0f, 1.0f, "%.02f");
        ImGui::SliderFloat("UV Scale", &cbData.uvScale, 1.0, 25.0f, "%.02f");
    }
    ImGui::End();
}

void Grass::DrawChunk(Graphics& gfx, const Chunk& chunk) const
{
    chunk.instanceBuffer->Bind(gfx);
    Bind(gfx);
    transform->SetTransform(DirectX::XMMatrixTranslation(chunk.position.x, 0.0f, chunk.position.y));
    Drawable::Draw(gfx, chunk.grassInstanceCount);
}

