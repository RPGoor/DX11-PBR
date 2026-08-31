#include "Terrain.h"
#include <MeshFactory.h>

Terrain::Terrain(Graphics& gfx)
{
    MeshData data = MeshFactory::Load("..\\..\\Assets\\Models\\terrain3.fbx");
    pipeline = std::make_unique<PipelineSettings>(gfx, VertexLayout<Vertex::Standard>::elements, "TerrainVS.cso", "StandardPS.cso");
    material = Material::Resolve(gfx, "M_terrain");
    mesh = Mesh::Resolve(gfx, data);
    transform = std::make_unique<TransformCbuf>(gfx);
}

void Terrain::DrawChunk(Graphics& gfx, const Chunk& chunk) const
{
    transform->SetTransform(DirectX::XMMatrixTranslation(chunk.position.x, 0.0f, chunk.position.y));
    Drawable::Draw(gfx);
}


