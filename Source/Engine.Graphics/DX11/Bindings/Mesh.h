#pragma once

#include "../../Core/MeshData.h"
#include "../Bindable.h"
#include "IndexBuffer.h"
#include "Topology.h"
#include "VertexBuffer.h"
#include <memory>

class Mesh : public Bindable
{
  public:
    Mesh(Graphics& gfx, MeshData data);

    virtual void Bind(Graphics& gfx) noexcept override;

    static std::shared_ptr<Mesh> Resolve(Graphics& gfx, MeshData data);
    std::string GetUID() const noexcept override;
    const UINT GetCount() const noexcept;

    template <typename... Ignore> static std::string GenerateUID(const MeshData data, Ignore&&... ignore)
    {
        return GenerateUID_(data.tag);
    }

  private:
    static std::string GenerateUID_(const std::string& tag);

  private:
    std::string tag;

    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::shared_ptr<Topology> topology;
};
