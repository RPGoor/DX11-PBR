#include "Scene.h"

Scene::Scene(Graphics& gfx)
    : pointLight(gfx), landscape(gfx)
{
    landscape.GenerateTerrain(gfx);
}

void Scene::Update(float dt)
{

}

void Scene::Draw(Graphics& gfx)
{
    pointLight.Bind(gfx, camera.GetMatrix());
    landscape.Draw(gfx, camera);
    landscape.SpawnControlWindows();
}

Camera& Scene::GetCamera() noexcept
{
    return camera;
}
