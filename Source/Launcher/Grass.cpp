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

    std::vector<InstanceData> instances;

    for (int i = 0; i < 2500000; ++i)
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
        DirectX::XMStoreFloat4x4(
            &storedMatrix,
            matrix
        );

        InstanceData instance{};

        instance.row0 = DirectX::XMFLOAT4(
            storedMatrix._11,
            storedMatrix._12,
            storedMatrix._13,
            storedMatrix._14
        );

        instance.row1 = DirectX::XMFLOAT4(
            storedMatrix._21,
            storedMatrix._22,
            storedMatrix._23,
            storedMatrix._24
        );

        instance.row2 = DirectX::XMFLOAT4(
            storedMatrix._31,
            storedMatrix._32,
            storedMatrix._33,
            storedMatrix._34
        );

        instance.row3 = DirectX::XMFLOAT4(
            storedMatrix._41,
            storedMatrix._42,
            storedMatrix._43,
            storedMatrix._44
        );

        instance.row4 = DirectX::XMFLOAT4(
            colorDistR(rng),
            colorDistG(rng),
            colorDistB(rng),
            1.0f
        );

        instances.push_back(instance);
    }

    instanceBuffer = std::make_unique<InstanceBuffer>(gfx, instances);

    noiseTexture = std::make_unique<Texture>(
        gfx,
        "..\\..\\Assets\\Textures\\noise2.png",
        1u
    );

    MeshData data = MeshFactory::Load("..\\..\\Assets\\Models\\grass.obj");
    renderSettings = std::make_unique<PipelineSettings>(gfx, data.vertices.GetLayout(), InstanceData::GetLayout(), "GrassVS.cso", "GrassPS.cso");
    material = std::make_unique<Material>(gfx, MaterialConstants { {0.15f, 0.63f, 0.23f}, 0.1, 0.75, {} });
    grass = std::make_unique<Mesh>(gfx, data);


    noiseSampler = std::make_unique<Sampler>(gfx, D3D11_TEXTURE_ADDRESS_WRAP, 1u);
}

void Grass::Draw(Graphics& gfx, DirectX::XMMATRIX position)
{
    Bind(gfx);
    grass->DrawInstanced(gfx, position, instanceBuffer->GetCount());
}

void Grass::Bind(Graphics& gfx)
{
    auto dataCopy = cbData;
    cbuf.Update(gfx, dataCopy);

    cbuf.Bind(gfx);
    noiseTexture->BindVS(gfx);
    noiseSampler->BindVS(gfx);
    renderSettings->Bind(gfx);
    material->Bind(gfx);
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
