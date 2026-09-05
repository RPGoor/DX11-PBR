#pragma once
#include "Landscape.h"
#include <Camera.h>
#include <DirectionalLight.h>

class Scene
{
  public:
    Scene(Graphics& gfx);
    void Update(float dt);
    void Draw(Graphics& gfx);
    Camera& GetCamera() noexcept;

  private:
    Landscape landscape;
    DirectionalLight pointLight;
    Camera camera;
};
