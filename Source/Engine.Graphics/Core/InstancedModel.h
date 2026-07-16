#pragma once

#include "Model.h"
#include "../DX11/Bindings/InstanceBuffer.h"

class InstancedModel
{
public:
    InstancedModel(Graphics& gfx, const std::string fileName, std::vector<InstanceData> instances);
    ~InstancedModel() = default;


public:
    void Draw(Graphics& gfx, DirectX::XMMATRIX position);
private:
    Model model;
    InstanceBuffer instanceBuffer;
};