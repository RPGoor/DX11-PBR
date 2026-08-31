#pragma once
#include <Graphics.h>
#include "Grass.h"
#include "Terrain.h"
#include <Bindings/TerrainComputeShader.h>
#include "Chunk.h"
#include <Camera.h>

class Landscape
{
public:
    Landscape(Graphics& gfx);
    void SpawnControlWindows() noexcept;

    void Draw(Graphics& gfx, Camera& cam) const;
    void GenerateTerrain(Graphics& gfx);

private:

    std::unique_ptr<TerrainComputeShader> terrainShader;

    Grass grass;
    Terrain terrain;

    std::vector<Chunk> chunks;
};