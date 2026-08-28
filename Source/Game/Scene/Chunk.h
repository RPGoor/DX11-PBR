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

    DirectX::XMFLOAT2 position;
    UINT grassInstanceCount;
    std::unique_ptr<InstanceBuffer> instanceBuffer;
    std::unique_ptr<ComputeTexture> heightmap;
    std::unique_ptr<ComputeTexture> normalmap;
private:
    void GenerateGrassInstances(Graphics& gfx);
    DirectX::BoundingBox bounds;


};
