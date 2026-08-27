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


