#include "InstancedModel.h"

InstancedModel::InstancedModel(Graphics& gfx, const std::string fileName, std::vector<InstanceData> instances, MaterialConstants material)
    : model(gfx, fileName, material, true ), instanceBuffer(gfx, std::move(instances))
{
}

void InstancedModel::Draw(Graphics & gfx, DirectX::XMMATRIX position)
{
    instanceBuffer.Bind(gfx);

    model.DrawInstanced(
        gfx,
        position,
        instanceBuffer.GetCount()
    );
}
