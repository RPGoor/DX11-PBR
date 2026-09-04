#include "../BindableCodex.h"
#include "Mesh.h"

Mesh::Mesh(Graphics& gfx, MeshData data)
    : tag(data.tag)
{
    topology = Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    vertexBuffer = std::make_unique<VertexBuffer>(gfx, data.tag, data.vertices);
    indexBuffer = std::make_unique<IndexBuffer>(gfx, data.tag, data.indices);
}

void Mesh::Bind(Graphics& gfx) noexcept
{
    vertexBuffer->Bind(gfx);
    indexBuffer->Bind(gfx);
    topology->Bind(gfx);
}

std::shared_ptr<Mesh> Mesh::Resolve(Graphics& gfx, MeshData data)
{
    return Codex::Resolve<Mesh>(gfx, data);
}

std::string Mesh::GenerateUID_(const std::string& tag)
{
    using namespace std::string_literals;
    return typeid(Mesh).name() + "#"s + tag;
}

std::string Mesh::GetUID() const noexcept
{
    return GenerateUID_(tag);
}

const UINT& Mesh::GetCount() const noexcept
{
    return indexBuffer->GetCount();
}
