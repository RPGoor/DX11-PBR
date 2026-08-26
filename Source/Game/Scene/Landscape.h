#pragma once
#include <Graphics.h>
#include "Grass.h"
#include "Terrain.h"

class Landscape
{
public:
    Landscape(Graphics& gfx);
    void SpawnControlWindows() noexcept;

    void Draw(Graphics& gfx) const;
    void GenerateTerrain(Graphics& gfx);

private:
    std::unique_ptr<TerrainComputeShader> terrainShader;

    Grass grass;
    Terrain terrain;


};