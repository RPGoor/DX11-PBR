#pragma once
#include "Window.h"
#include <Time/Timer.h>
#include <ImGuiManager.h>
#include <Graphics.h>
#include <Model.h>
#include <Camera.h>
#include <PointLight.h>
#include <Bindings/FrameCBuf.h>

class Game
{
public:
    Game();
    ~Game();
    int Go();
private:
    void DoFrame();

private:
    ImGuiManager imgui;
    Window wnd;
    Graphics gfx;
    Engine::Timer timer;
    Camera cam;
    PointLight pointLight;
    FrameCbuf frameBuffer;

    //Model nano{ gfx,"..\\..\\Assets\\Models\\nano_textured\\nanosuit.obj" };
    Model grid{ gfx, "..\\..\\Assets\\Models\\grid.obj" };
    Model grass{ gfx, "..\\..\\Assets\\Models\\grass.obj" };

    std::vector<DirectX::XMFLOAT4X4> grassTransforms;
};
