#pragma once
#include "Window.h"
#include <Time/Timer.h>
#include <ImGuiManager.h>
#include <Graphics.h>

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
    std::unique_ptr<Graphics> gfx;

    Engine::Timer timer;
};

