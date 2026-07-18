#pragma once

#include "Bindable.h"
#include "../../Core/MaterialConstants.h"
#include "ConstantBuffers.h"

class MaterialBuffer : public Bindable
{
public:
    MaterialBuffer(Graphics& gfx, MaterialConstants material);

    void Bind(Graphics& gfx) noexcept override;

private:
    MaterialConstants cbData;
    PixelConstantBuffer<MaterialConstants> cbuf;
};