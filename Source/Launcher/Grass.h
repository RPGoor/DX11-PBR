#pragma once

#include <InstancedModel.h>
#include <Bindings/Texture.h>
#include <Bindings/Sampler.h>
#include <Bindings/ConstantBuffers.h>

class Grass
{
public:
    Grass(Graphics& gfx);
    void Draw(Graphics& gfx, DirectX::XMMATRIX position);
    void Bind(Graphics& gfx);
    void SpawnControlWindow() noexcept;
private:
    struct alignas(16) GrassConstants
    {
        float horizontalBendStrength;
        float verticalBendStrength;
        float bendMaskPow;
        float speed;
        float direction[2];
        float uvScale;
        float padding;
    };

    GrassConstants cbData;
    mutable VertexConstantBuffer<GrassConstants> cbuf;

    std::unique_ptr<InstancedModel> grass;
    std::unique_ptr<Texture> noiseTexture;
    std::unique_ptr<Sampler> noiseSampler;
};