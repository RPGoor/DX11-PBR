#include "Landscape.h"

Landscape::Landscape(Graphics& gfx)
    : grass(gfx), terrain(gfx)
{
    terrainShader = std::make_unique<TerrainComputeShader>(gfx);
    terrainShader->regenCallback = [this, &gfx]
    {
        GenerateTerrain(gfx);
    };
}

void Landscape::SpawnControlWindows() noexcept
{

    grass.SpawnControlWindow();
    terrainShader->SpawnControlWindow();
}

void Landscape::Draw(Graphics& gfx) const
{
    terrainShader->BindVS(gfx);
    terrain.Draw(gfx);
    grass.Draw(gfx);
}

void Landscape::GenerateTerrain(Graphics& gfx)
{
    terrainShader->Generate(gfx);
}