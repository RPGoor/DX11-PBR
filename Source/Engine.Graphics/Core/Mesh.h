#pragma once

#include <ConditionalNoexcept.h>
#include "../DX11/Bindings/Bindable.h"
#include "Drawable.h"

class Mesh : public Drawable
{
public:
    Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bindable>> bindPtrs);
    void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const conexcept;
    void DrawInstanced(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform, UINT instanceCount) const conexcept;

    DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
    mutable DirectX::XMFLOAT4X4 transform;
};