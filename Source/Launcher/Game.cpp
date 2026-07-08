#include "Game.h"

Game::Game()
    : wnd(800, 600, L"Asole Engine Window")
{
}

Game::~Game()
{}

int Game::Go()
{
    while (true)
    {
        if (const auto ecode = Window::ProcessMessagePump())
        {
            return *ecode;
        }
        DoFrame();
    }
}

void Game::DoFrame()
{
    const auto dt = timer.Mark();
}
