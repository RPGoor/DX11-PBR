#pragma once

#include "Chunk.h"
#include <Bindings/ConstantBuffers.h>
#include <Bindings/InstanceBuffer.h>
#include <Bindings/Material.h>
#include <Bindings/Mesh.h>
#include <Bindings/PipelineSettings.h>
#include <Bindings/Sampler.h>
#include <Bindings/Texture.h>
#include <Drawable.h>

class Grass : public Drawable
{
  public:
    Grass(Graphics& gfx);
    void Bind(Graphics& gfx) const;
    void SpawnControlWindow() noexcept;
    UINT GetIndexCount() noexcept;
    void DrawChunk(Graphics& gfx, const Chunk& chunk) const;

  private:
    struct alignas(16) GrassConstants
    {
        float bendStrength;
        float bendMaskPow;
        float speed;
        float padding0;
        float direction[2];
        float uvScale;
        float padding1;
    };

    GrassConstants cbData;
    mutable VertexConstantBuffer<GrassConstants> cbuf;

    std::unique_ptr<Texture> noiseTexture;
    std::unique_ptr<Sampler> noiseSampler;
};
