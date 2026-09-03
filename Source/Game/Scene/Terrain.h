#pragma once

#include <Graphics.h>

#include <Bindings/Sampler.h>
#include "Grass.h"
#include "Chunk.h"

class Terrain : public Drawable
{

public:
    Terrain(Graphics& gfx);
    void DrawChunk(Graphics& gfx, const Chunk& chunk) const;
private:
    std::unique_ptr<Texture> rockTexture;
    std::unique_ptr<Texture> dirtTexture;
    std::unique_ptr<Sampler> sampler;
};