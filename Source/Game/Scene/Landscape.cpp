#include "Landscape.h"

Landscape::Landscape(Graphics& gfx)
    : grass(gfx), terrain(gfx)
{
    terrainShader = std::make_unique<TerrainComputeShader>(gfx);
    grassShader = std::make_unique<GrassComputeShader>(gfx);
    terrainShader->regenCallback = [this, &gfx]
    {
        GenerateTerrain(gfx);
    };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            chunks.emplace_back(Chunk(gfx, { (float)i, (float)j }, *terrainShader, *grassShader, grass.GetIndexCount()));
        }
    }
}

void Landscape::SpawnControlWindows() noexcept
{

    grass.SpawnControlWindow();
    terrainShader->SpawnControlWindow();
}

void Landscape::Draw(Graphics& gfx, Camera& cam) const
{
    terrainShader->BindVS(gfx);
    for (const Chunk& chunk : chunks)
    {
        if (!cam.GetFrustum().Intersects(chunk.bounds))
        {
            continue;
        }

        chunk.Bind(gfx);
        terrain.DrawChunk(gfx, chunk);

        if (chunk.SqrDistanceTo(cam.pos) > 50.0f * 50.0f)
        {
            continue;
        }
        chunk.BindInstanceData(gfx);
        grass.DrawChunk(gfx, chunk);
    }
}

void Landscape::GenerateTerrain(Graphics& gfx)
{
    for (const Chunk& chunk : chunks)
    {
        terrainShader->Generate(gfx, *chunk.heightmap, *chunk.normalmap, chunk.position);
        grassShader->Generate(gfx, *chunk.instanceBuffer, *chunk.indirectGrassArgs, chunk.position, *chunk.normalmap);
    }
}