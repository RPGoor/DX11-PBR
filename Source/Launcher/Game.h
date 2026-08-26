#pragma once
#include "Window.h"
#include <Time/Timer.h>
#include <ImGuiManager.h>
#include <Graphics.h>
#include <Camera.h>
#include <PointLight.h>
#include <Bindings/FrameCBuf.h>
#include "Landscape.h"


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

    Landscape landscape;
};
