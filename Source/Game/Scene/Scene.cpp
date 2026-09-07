#include "Scene.h"
#include <MeshFactory.h>

Scene::Scene(Graphics& gfx)
    : pointLight(gfx)
// landscape(gfx)
{
    // landscape.GenerateTerrain(gfx);
    constexpr int count = 5;
    constexpr float spacing = 2.5f;

    for (int y = 0; y < count; ++y)
    {
        for (int x = 0; x < count; ++x)
        {
            float metallic = 1.0f - static_cast<float>(y) / (count - 1);

            float roughness = static_cast<float>(x) / (count - 1);

            spheres.emplace_back(
                std::make_unique<Sphere>(gfx, DirectX::XMFLOAT3{x * spacing, y * spacing, 0.0f}, metallic, roughness)
            );
        }
    }
}

void Scene::Update(float dt) {}

void Scene::Draw(Graphics& gfx)
{
    pointLight.Bind(gfx, camera.GetMatrix());
    for (auto& sphere : spheres)
    {
        sphere->Draw(gfx);
    }
    // landscape.Draw(gfx, camera);
    // landscape.SpawnControlWindows();
}

Scene::Sphere::Sphere(Graphics& gfx, DirectX::XMFLOAT3 position, float metallic, float roughness)
{
    MeshData data = MeshFactory::Load("..\\..\\Assets\\Models\\sphere.obj");
    pipeline = std::make_unique<PipelineSettings>(
        gfx,
        Vertex::CombineLayouts(VertexLayout<Vertex::Standard>::elements, VertexLayout<Vertex::Instance>::elements),
        "StandardVS.cso",
        "StandardPS.cso"
    );
    material = Material::Resolve(
        gfx,
        "M_sphere" + std::to_string(metallic) + std::to_string(roughness),
        MaterialConstants{{0.15f, 0.63f, 0.23f}, metallic, roughness, {}}
    );
    mesh = Mesh::Resolve(gfx, data);
    transform = std::make_unique<TransformCbuf>(gfx);

    transform->SetTransform(DirectX::XMMatrixTranslation(position.x, position.y, position.z));
    // sphere mesh
    // PBR material
    // transform

    // material metallic  = metallic;
    // material roughness = roughness;
}

Camera& Scene::GetCamera() noexcept
{
    return camera;
}
