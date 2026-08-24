#pragma once

#include "../DX11/Bindings/InstanceBuffer.h"
#include "../Core/Material.h"

class InstancedModel
{
public:
    InstancedModel(Graphics& gfx, const std::string fileName, std::vector<InstanceData> instances, MaterialConstants material);
    ~InstancedModel() = default;


public:
    void Draw(Graphics& gfx, DirectX::XMMATRIX position);
private:
    //Model model;
    InstanceBuffer instanceBuffer;
};