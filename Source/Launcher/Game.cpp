#include "Game.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

Game::Game()
    : wnd(800, 600, L"Solorn Engine Window")
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
