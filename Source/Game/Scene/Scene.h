#pragma once
#include "Landscape.h"
#include <PointLight.h>
#include <Camera.h>

class Scene
{
public:
    Scene(Graphics& gfx);
    void Update(float dt);
    void Draw(Graphics& gfx);
    Camera& GetCamera() noexcept;
private:
    Landscape landscape;
    PointLight pointLight;
    Camera camera;

};