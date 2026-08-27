#pragma once
#include "Window.h"
#include <Time/Timer.h>
#include <ImGuiManager.h>
#include <Graphics.h>
#include <Bindings/FrameCBuf.h>
#include "Controller.h"
#include "../Scene/Scene.h"


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
    FrameCbuf frameBuffer;

    Controller controller;
    Scene scene;
};
