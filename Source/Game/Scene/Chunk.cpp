#include "Chunk.h"
#include <random>

Chunk::Chunk(Graphics& gfx, DirectX::XMFLOAT2 position, TerrainComputeShader& terrainShader)
    : position(position.x * 20, position.y * 20), grassInstanceCount(400000)
{
    heightmap = std::make_unique<ComputeTexture>(gfx, 512u, 512u, 0u, 0u, DXGI_FORMAT_R32G32B32A32_FLOAT);
    normalmap = std::make_unique<ComputeTexture>(gfx, 512u, 512u, 1u, 1u, DXGI_FORMAT_R32G32B32A32_FLOAT);

    bounds.Center = {Chunk::position.x, 0.0f, Chunk::position.y};

    bounds.Extents = { 12.0f, 50.0f, 12.0f };

    terrainShader.Generate(gfx, *heightmap, *normalmap, Chunk::position);
    GenerateGrassInstances(gfx);
}

void Chunk::Bind(Graphics& gfx) const
{
    normalmap->BindVS(gfx);
    heightmap->BindVS(gfx);
}

float Chunk::SqrDistanceTo(const DirectX::XMFLOAT3& pos) const
{
    const float dx = pos.x - bounds.Center.x;
    const float dz = pos.z - bounds.Center.z;

    return dx * dx + dz * dz;
}

void Chunk::GenerateGrassInstances(Graphics& gfx)
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

    std::uniform_real_distribution<float> LeanDist(
        -0.15f,
        0.15f
    );

    std::vector<Vertex::Instance> instances;
    instances.reserve(grassInstanceCount);
    for (int i = 0; i < grassInstanceCount; ++i)
    {
        const float x = positionDist(rng);
        const float z = positionDist(rng);

        const float rotation = rotationDist(rng);

        const float scale = scaleDist(rng);

        instances.push_back({
            {x, z},
            rotation,
            scale
        });
    }
    instanceBuffer = std::make_unique<InstanceBuffer>(gfx, instances);
}