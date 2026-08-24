#pragma once

#include <InstancedModel.h>
#include <Bindings/Texture.h>
#include <Bindings/Sampler.h>
#include <Bindings/ConstantBuffers.h>
#include <Mesh.h>
#include <PipelineSettings.h>
#include <Material.h>

class Grass
{
public:
    Grass(Graphics& gfx);
    void Draw(Graphics& gfx, DirectX::XMMATRIX position);
    void Bind(Graphics& gfx);
    void SpawnControlWindow() noexcept;
private:
    std::unique_ptr<Mesh> grass;
    std::unique_ptr<PipelineSettings> renderSettings;
    std::unique_ptr<Material> material;

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

    std::unique_ptr<InstanceBuffer> instanceBuffer;
    std::unique_ptr<Texture> noiseTexture;
    std::unique_ptr<Sampler> noiseSampler;
};