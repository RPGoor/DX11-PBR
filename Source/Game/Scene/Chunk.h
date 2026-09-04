#pragma once
#include <Bindings/ComputeTexture.h>
#include <Bindings/GrassComputeShader.h>
#include <Bindings/IndirectArgsBuffer.h>
#include <Bindings/InstanceBuffer.h>
#include <Bindings/TerrainComputeShader.h>
#include <directxcollision.h>
#include <memory>

class Chunk
{
  public:
    Chunk(
        Graphics& gfx,
        DirectX::XMFLOAT2 position,
        TerrainComputeShader& terrainShader,
        GrassComputeShader& grassShader,
        UINT grassIndexCount
    );
    void Bind(Graphics& gfx) const;
    void BindInstanceData(Graphics& gfx) const;
    float SqrDistanceTo(const DirectX::XMFLOAT3& pos) const;

    DirectX::XMFLOAT2 position;
    UINT grassInstanceCount;
    std::unique_ptr<InstanceBuffer> instanceBuffer;

    std::unique_ptr<IndirectArgsBuffer> indirectGrassArgs;

    std::unique_ptr<ComputeTexture> heightmap;
    std::unique_ptr<ComputeTexture> normalmap;
    DirectX::BoundingBox bounds;

  private:
};
