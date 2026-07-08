#pragma once
#include "Window.h"
#include <Time/Timer.h>

class Game
{
public:
    Game();
    ~Game();
    int Go();
private:
    void DoFrame();

    Window wnd;
    Engine::Timer timer;
};

