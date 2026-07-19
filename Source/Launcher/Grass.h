#pragma once

#include <InstancedModel.h>
#include <Bindings/Texture.h>
#include <Bindings/Sampler.h>

class Grass
{
public:
    Grass(Graphics& gfx);
    void Draw(Graphics& gfx, DirectX::XMMATRIX position);
    void Bind(Graphics& gfx);
private:
    std::unique_ptr<InstancedModel> grass;
    std::unique_ptr<Texture> noiseTexture;
    std::unique_ptr<Sampler> noiseSampler;
};