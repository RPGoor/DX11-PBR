#pragma once

#include <Graphics.h>
#include <Bindings/ComputeTexture.h>
#include <Bindings/TerrainComputeShader.h>
#include <Bindings/Sampler.h>
#include "Grass.h"
#include <Mesh.h>
#include <PipelineSettings.h>
#include <Material.h>

class Terrain : public Drawable
{

public:
    Terrain(Graphics& gfx);
    void SpawnControlWindow() noexcept;

    virtual void Draw(Graphics& gfx) const override;
    virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;

    void GenerateTerrain(Graphics& gfx);
private:
    std::unique_ptr<Grass> grass;
    std::unique_ptr<TerrainComputeShader> terrainShader;
};