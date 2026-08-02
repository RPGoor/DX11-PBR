#pragma once

#include <Graphics.h>
#include <Bindings/ComputeTexture.h>
#include <Bindings/ComputeShader.h>
#include <Model.h>
#include <Bindings/Sampler.h>
#include "Grass.h"

class Terrain
{

public:
    Terrain(Graphics& gfx);
    void Generate(Graphics& gfx);
    void Draw(Graphics& gfx, DirectX::XMMATRIX position);
    void SpawnControlWindow() noexcept;

private:
    static constexpr UINT HeightmapResolution = 1024u;
    ComputeTexture heightmap;
    ComputeShader terrainShader;

    std::unique_ptr<Model> terrain;
    std::unique_ptr<Sampler> terrainSampler;
    std::unique_ptr<Grass> grass;
};