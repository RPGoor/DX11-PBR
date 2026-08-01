#pragma once
#include "Window.h"
#include <Time/Timer.h>
#include <ImGuiManager.h>
#include <Graphics.h>
#include <Model.h>
#include <Camera.h>
#include <PointLight.h>
#include <Bindings/FrameCBuf.h>
#include <InstancedModel.h>
#include <../DX11/Bindings/MaterialBuffer.h>
#include "Grass.h"
#include "Terrain.h"


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

    // Model grid{ gfx, "..\\..\\Assets\\Models\\grid.obj", MaterialConstants{ {0.3f, 0.2f, 0.1f}, 0.05, 0.75, {} } };
    Terrain terrain{ gfx };
};
