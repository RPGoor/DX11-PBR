#pragma once
#include <directxcollision.h>
#include <memory>
#include <Bindings/ComputeTexture.h>
#include <Bindings/InstanceBuffer.h>
#include <Bindings/TerrainComputeShader.h>

class Chunk
{
public:
    Chunk(Graphics& gfx, DirectX::XMFLOAT2 position, TerrainComputeShader& terrainShader);
    void Bind(Graphics& gfx) const;
    float SqrDistanceTo(const DirectX::XMFLOAT3& pos) const;


    DirectX::XMFLOAT2 position;
    UINT grassInstanceCount;
    std::unique_ptr<InstanceBuffer> instanceBuffer;
    std::unique_ptr<ComputeTexture> heightmap;
    std::unique_ptr<ComputeTexture> normalmap;
    DirectX::BoundingBox bounds;
private:
    void GenerateGrassInstances(Graphics& gfx);


};
