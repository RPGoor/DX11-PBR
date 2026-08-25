#include "Terrain.h"
#include <MeshFactory.h>

Terrain::Terrain(Graphics& gfx)
{

    MeshData data = MeshFactory::Load("..\\..\\Assets\\Models\\terrain3.fbx");
    pipeline = std::make_unique<PipelineSettings>(gfx, data.vertices.GetLayout(), "TerrainVS.cso", "StandardPS.cso");
    material = Material::Resolve(gfx, "M_terrain");
    mesh = Mesh::Resolve(gfx, data);

    grass = std::make_unique<Grass>(gfx);

    terrainShader = std::make_unique<TerrainComputeShader>(gfx);
    terrainShader->regenCallback = [this, &gfx]
    {
        GenerateTerrain(gfx);
    };
}

void Terrain::Draw(Graphics& gfx) const
{
    mesh->Bind(gfx);
    pipeline->Bind(gfx);
    material->Bind(gfx);
    terrainShader->BindVS(gfx);
    gfx.DrawIndexed(mesh->GetCount());

    grass->Draw(gfx);
}

DirectX::XMMATRIX Terrain::GetTransformXM() const noexcept
{
    return DirectX::XMMatrixIdentity();
}

void Terrain::SpawnControlWindow() noexcept
{
    terrainShader->SpawnControlWindow();
    grass->SpawnControlWindow();
}

void Terrain::GenerateTerrain(Graphics& gfx)
{
    terrainShader->Generate(gfx);
}
