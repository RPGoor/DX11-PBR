#include "Grass.h"
#include <random>

Grass::Grass(Graphics& gfx)
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

    grass = std::make_unique<InstancedModel>(
        gfx,
        "..\\..\\Assets\\Models\\grass.obj",
        instances,
        MaterialConstants{ {0.15f, 0.63f, 0.23f}, 0.1, 0.5, {} }
    );

    noiseTexture = std::make_unique<Texture>(
        gfx,
        "..\\..\\Assets\\Textures\\noise.png"
    );

    noiseSampler = std::make_unique<Sampler>(gfx);
}

void Grass::Draw(Graphics& gfx, DirectX::XMMATRIX position)
{
    Bind(gfx);
    grass->Draw(gfx, position);
}

void Grass::Bind(Graphics& gfx)
{
    noiseTexture->BindVS(gfx);
    noiseSampler->BindVS(gfx);
}
