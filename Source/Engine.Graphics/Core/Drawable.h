#pragma once
#include "../DX11/Graphics.h"
#include "../DX11/Bindings/Bindable.h"
#include <memory>
#include <vector>
#include "../DX11/Bindings/IndexBuffer.h"
#include "../DX11/Bindings/TransformCbuf.h"
#include "Mesh.h"
#include "PipelineSettings.h"
#include "Material.h"

class Drawable
{
public:
    Drawable(const Drawable&) = delete;
    virtual ~Drawable() = default;

public:
    void Draw(Graphics& gfx) const;

protected:
    Drawable();

    virtual void DrawCall(Graphics& gfx) const;

    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    std::shared_ptr<PipelineSettings> pipeline;
    std::unique_ptr<TransformCbuf> transform;

private:

};