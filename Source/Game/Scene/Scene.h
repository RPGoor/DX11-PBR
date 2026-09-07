#pragma once
#include "Landscape.h"
#include <Camera.h>
#include <DirectionalLight.h>
#include <memory>
#include <vector>

class Scene
{
  public:
    Scene(Graphics& gfx);
    void Update(float dt);
    void Draw(Graphics& gfx);
    Camera& GetCamera() noexcept;

  private:
    // Landscape landscape;
    class Sphere : public Drawable
    {
      public:
        Sphere(Graphics& gfx, DirectX::XMFLOAT3 position, float metallic, float roughness);
    };

    std::vector<std::unique_ptr<Sphere>> spheres;
    DirectionalLight pointLight;
    Camera camera;
};
