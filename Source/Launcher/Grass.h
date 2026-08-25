#pragma once

#include <Bindings/InstanceBuffer.h>
#include <Bindings/Texture.h>
#include <Bindings/Sampler.h>
#include <Bindings/ConstantBuffers.h>
#include <Mesh.h>
#include <PipelineSettings.h>
#include <Material.h>
#include <Drawable.h>

class Grass : public Drawable
{
public:
    Grass(Graphics& gfx);
    virtual void Draw(Graphics& gfx) const override;
    void Bind(Graphics& gfx) const;
    void SpawnControlWindow() noexcept;

    virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;
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

    std::unique_ptr<InstanceBuffer> instanceBuffer;
    std::unique_ptr<Texture> noiseTexture;
    std::unique_ptr<Sampler> noiseSampler;
};