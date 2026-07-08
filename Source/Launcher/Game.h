#pragma once
#include "Window.h"
#include <Time/Timer.h>
#include <ImGuiManager.h>

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
    Engine::Timer timer;
};

