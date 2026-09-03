#include "Chunk.h"
#include <random>

Chunk::Chunk(Graphics& gfx, DirectX::XMFLOAT2 position, TerrainComputeShader& terrainShader, GrassComputeShader& grassShader, UINT grassIndexCount)
    : position(position.x * 20, position.y * 20), grassInstanceCount(400000)
{
    heightmap = std::make_unique<ComputeTexture>(gfx, 512u, 512u, 0u, 0u, DXGI_FORMAT_R32G32B32A32_FLOAT);
    normalmap = std::make_unique<ComputeTexture>(gfx, 512u, 512u, 1u, 1u, DXGI_FORMAT_R32G32B32A32_FLOAT);

    bounds.Center = {Chunk::position.x, 0.0f, Chunk::position.y};

    bounds.Extents = { 12.0f, 50.0f, 12.0f };

    terrainShader.Generate(gfx, *heightmap, *normalmap, Chunk::position);
    instanceBuffer = std::make_unique<InstanceBuffer>(gfx, grassInstanceCount);
    indirectGrassArgs = std::make_unique<IndirectArgsBuffer>(gfx, grassIndexCount);
    grassShader.Generate(gfx, *instanceBuffer, *indirectGrassArgs, Chunk::position, *normalmap);
}

void Chunk::Bind(Graphics& gfx) const
{
    normalmap->BindVS(gfx);
    heightmap->BindVS(gfx);
}

void Chunk::BindInstanceData(Graphics& gfx) const
{
    instanceBuffer->Bind(gfx);
}

float Chunk::SqrDistanceTo(const DirectX::XMFLOAT3& pos) const
{
    const float dx = pos.x - bounds.Center.x;
    const float dz = pos.z - bounds.Center.z;

    return dx * dx + dz * dz;
}
