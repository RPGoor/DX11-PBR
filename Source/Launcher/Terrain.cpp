#include "Terrain.h"

Terrain::Terrain(Graphics& gfx)
    :
    heightmap(gfx, HeightmapResolution, HeightmapResolution, 0u, 0u),
    normalmap(gfx, HeightmapResolution, HeightmapResolution, 1u, 1u, DXGI_FORMAT_R32G32B32A32_FLOAT),
    terrainShader(gfx, "TerrainCS.cso")
{
    terrain = std::make_unique<Model>(
        gfx,
        "..\\..\\Assets\\Models\\terrain2.fbx",
        MaterialConstants{ {0.15f, 0.63f, 0.23f}, 0.1, 0.75, {} }
        );

    grass = std::make_unique<Grass>(gfx);

    terrainSampler = std::make_unique<Sampler>(gfx, D3D11_TEXTURE_ADDRESS_CLAMP);

    terrainShader.regenCallback = [this, &gfx]
    {
        Generate(gfx);
    };
}

void Terrain::Generate(Graphics& gfx)
{
    terrainShader.Bind(gfx);
    heightmap.BindUAV(gfx);
    normalmap.BindUAV(gfx);

    constexpr UINT threadGroupSize = 8u;

    const UINT groupCount =
        (HeightmapResolution + threadGroupSize - 1u) /
        threadGroupSize;

    gfx.Dispatch(
        groupCount,
        groupCount,
        1u
    );

    heightmap.UnbindUAV(gfx);
    normalmap.UnbindUAV(gfx);
    terrainShader.Unbind(gfx);
}

void Terrain::Draw(Graphics& gfx, DirectX::XMMATRIX position)
{
    heightmap.BindVS(gfx);
    normalmap.BindVS(gfx);
    terrainSampler->BindVS(gfx);
    terrain->Draw(gfx, position);
    grass->Draw(gfx, DirectX::XMMatrixIdentity());
}

void Terrain::DrawShadow(Graphics& gfx, DirectX::XMMATRIX position)
{
    heightmap.BindVS(gfx);
    normalmap.BindVS(gfx);
    terrainSampler->BindVS(gfx);
    terrain->Draw(gfx, position);
    grass->Draw(gfx, DirectX::XMMatrixIdentity());
}

void Terrain::SpawnControlWindow() noexcept
{
    terrainShader.SpawnControlWindow();
    grass->SpawnControlWindow();
}
