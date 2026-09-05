#pragma once
#include "../Scene/Scene.h"
#include "Controller.h"
#include "Window.h"
#include <Bindings/FrameCBuf.h>
#include <Graphics.h>
#include <ImGuiManager.h>
#include <Time/Timer.h>

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
