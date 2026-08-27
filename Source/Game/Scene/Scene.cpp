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
    landscape.Draw(gfx);
    landscape.SpawnControlWindows();
}

Camera& Scene::GetCamera() noexcept
{
    return camera;
}
