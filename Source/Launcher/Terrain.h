#pragma once

#include <Graphics.h>
#include <Bindings/ComputeTexture.h>
#include <Bindings/ComputeShader.h>
#include <Bindings/Sampler.h>
#include "Grass.h"
#include <Mesh.h>
#include <PipelineSettings.h>
#include <Material.h>

class Terrain
{

public:
    Terrain(Graphics& gfx);
    void Generate(Graphics& gfx);
    void Draw(Graphics& gfx, DirectX::XMMATRIX position);
    void SpawnControlWindow() noexcept;

private:
    std::unique_ptr<Mesh> terrain;
    std::unique_ptr<PipelineSettings> renderSettings;
    std::unique_ptr<Material> material;


    //static constexpr UINT HeightmapResolution = 1024u;
    //ComputeTexture heightmap;
    //ComputeTexture normalmap;
    //ComputeShader terrainShader;
    //std::unique_ptr<Sampler> terrainSampler;
    //std::unique_ptr<Grass> grass;
};