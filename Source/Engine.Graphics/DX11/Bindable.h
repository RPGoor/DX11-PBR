#pragma once
#include "Graphics.h"

class Bindable
{
public:
    virtual void Bind(Graphics& gfx) noexcept = 0;
    virtual ~Bindable() = default;

public:
    virtual std::string GetUID() const noexcept;

protected:
    static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
    static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
    static DxgiInfoManager& GetInfoManager(Graphics& gfx) conexcept;
};