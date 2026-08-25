#pragma once
#include "../DX11/Graphics.h"
#include "../DX11/Bindings/Bindable.h"
#include <memory>
#include <vector>
#include "../DX11/Bindings/IndexBuffer.h"
#include "Mesh.h"
#include "PipelineSettings.h"
#include "Material.h"

class Drawable
{
public:
    Drawable(const Drawable&) = delete;
    virtual ~Drawable() = default;

public:
    virtual void Draw(Graphics& gfx) const = 0;
    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

protected:
    Drawable() = default;

    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    std::shared_ptr<PipelineSettings> pipeline;
private:

};