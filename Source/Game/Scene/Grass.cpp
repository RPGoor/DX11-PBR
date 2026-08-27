#include "Grass.h"
#include <random>
#include "ImGui.h"
#include <MeshFactory.h>

Grass::Grass(Graphics& gfx)
    : cbuf(gfx, 2u), cbData(0.15f, 0.1f, 1.5f, 5.0f, {1.0f, 1.0f}, 8.0f, 0.0f)
{
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_real_distribution<float> positionDist(
        -10.0f,
        10.0f
    );

    std::uniform_real_distribution<float> rotationDist(
        0.0f,
        DirectX::XM_2PI
    );

    std::uniform_real_distribution<float> scaleDist(
        0.6f,
        1.4f
    );
    std::uniform_real_distribution<float> heightDist(
        0.6f,
        1.5f
    );

    std::uniform_real_distribution<float> colorDistR(
        0.75f,
        1.4f
    );
    std::uniform_real_distribution<float> colorDistG(
        0.8f,
        1.2f
    );
    std::uniform_real_distribution<float> colorDistB(
        0.85f,
        1.4f
    );

    std::uniform_real_distribution<float> LeanDist(
        -0.15f,
        0.15f
    );

    std::vector<Vertex::Instance> instances;

    for (int i = 0; i < 2500; ++i)
    {
        const float x = positionDist(rng);
        const float z = positionDist(rng);
        const float rotation = rotationDist(rng);
        const float scale = scaleDist(rng);

        const DirectX::XMMATRIX matrix =
            DirectX::XMMatrixScaling(
                scale,
                heightDist(rng),
                scale
            )
            *
            DirectX::XMMatrixRotationY(rotation)
            *
            DirectX::XMMatrixRotationX(LeanDist(rng))
            *
            DirectX::XMMatrixRotationZ(LeanDist(rng))
            *
            DirectX::XMMatrixTranslation(
                x,
                0.0f,
                z
            );

        DirectX::XMFLOAT4X4 storedMatrix;
        DirectX::XMStoreFloat4x4(&storedMatrix,matrix);

        Vertex::Instance instance
        {
            storedMatrix,
            DirectX::XMFLOAT4(colorDistR(rng), colorDistG(rng), colorDistB(rng), 1.0f)
        };

        instances.push_back(instance);
    }

    instanceBuffer = std::make_unique<InstanceBuffer>(gfx, instances);

    noiseTexture = std::make_unique<Texture>(
        gfx,
        "..\\..\\Assets\\Textures\\noise2.png",
        1u
    );

    MeshData data = MeshFactory::Load("..\\..\\Assets\\Models\\pampas.fbx");
    pipeline = std::make_unique<PipelineSettings>(
        gfx,
        Vertex::CombineLayouts(VertexLayout<Vertex::Standard>::elements, VertexLayout<Vertex::Instance>::elements),
        "GrassVS.cso",
        "GrassPS.cso");
    material = Material::Resolve(gfx, "M_grass", MaterialConstants{{0.15f, 0.63f, 0.23f}, 0.1, 0.75, {}});
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
    instanceBuffer->Bind(gfx);
}

void Grass::SpawnControlWindow() noexcept
{
    if (ImGui::Begin("Grass"))
    {
        ImGui::SliderFloat("Horizontal Strength", &cbData.horizontalBendStrength, 0.01f, 1.0f, "%.02f");
        ImGui::SliderFloat("Vertical Strength", &cbData.verticalBendStrength, 0.01, 1.0f, "%.02f");
        ImGui::SliderFloat("Mask Power", &cbData.bendMaskPow, 1.0, 4.0f, "%.02f");
        ImGui::SliderFloat("Speed", &cbData.speed, 0.01, 25.0f, "%.02f");
        ImGui::SliderFloat2("Direction", cbData.direction, -1.0f, 1.0f, "%.02f");
        ImGui::SliderFloat("UV Scale", &cbData.uvScale, 1.0, 25.0f, "%.02f");

    }
    ImGui::End();
}

void Grass::DrawCall(Graphics& gfx) const
{
    Bind(gfx);
    gfx.DrawIndexed(mesh->GetCount(), 2500u);
}

