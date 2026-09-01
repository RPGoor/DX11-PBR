#include "Terrain.h"
#include <MeshFactory.h>

Terrain::Terrain(Graphics& gfx)
{
    rockTexture = std::make_unique<Texture>(gfx, "..\\..\\Assets\\Textures\\rock_diff.jpg", 0u);
    dirtTexture = std::make_unique<Texture>(gfx, "..\\..\\Assets\\Textures\\dirt_diff.jpg", 1u);
    sampler = std::make_unique<Sampler>(gfx, D3D11_TEXTURE_ADDRESS_WRAP, 0u);

    MeshData data = MeshFactory::Load("..\\..\\Assets\\Models\\terrain3.fbx");
    pipeline = std::make_unique<PipelineSettings>(gfx, VertexLayout<Vertex::Standard>::elements, "TerrainVS.cso", "TerrainPS.cso");
    material = Material::Resolve(gfx, "M_terrain", { {0.2f, 0.1f, 0.03f}, 0.1f, 0.78f });
    mesh = Mesh::Resolve(gfx, data);
    transform = std::make_unique<TransformCbuf>(gfx);
}

void Terrain::DrawChunk(Graphics& gfx, const Chunk& chunk) const
{
    rockTexture->Bind(gfx);
    dirtTexture->Bind(gfx);
    sampler->Bind(gfx);
    transform->SetTransform(DirectX::XMMatrixTranslation(chunk.position.x, 0.0f, chunk.position.y));
    Drawable::Draw(gfx);
}


