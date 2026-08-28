#include "Landscape.h"

Landscape::Landscape(Graphics& gfx)
    : grass(gfx), terrain(gfx)
{
    terrainShader = std::make_unique<TerrainComputeShader>(gfx);
    terrainShader->regenCallback = [this, &gfx]
    {
        GenerateTerrain(gfx);
    };
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            chunks.emplace_back(Chunk(gfx, { (float)i, (float)j }, *terrainShader));
        }
    }
}

void Landscape::SpawnControlWindows() noexcept
{

    grass.SpawnControlWindow();
    terrainShader->SpawnControlWindow();
}

void Landscape::Draw(Graphics& gfx) const
{
    terrainShader->BindVS(gfx);
    for (const Chunk& chunk : chunks)
    {
        chunk.Bind(gfx);
        terrain.DrawChunk(gfx, chunk);
        grass.DrawChunk(gfx, chunk);
    }
}

void Landscape::GenerateTerrain(Graphics& gfx)
{
    for (const Chunk& chunk : chunks)
    {
        terrainShader->Generate(gfx, *chunk.heightmap, *chunk.normalmap, chunk.position);
    }
}