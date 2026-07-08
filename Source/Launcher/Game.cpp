#include "Game.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <GDIPlusManager.h>

GDIPlusManager gdipm;

Game::Game()
    : wnd(800, 600, L"Solorn Engine Window")
{
    gfx = std::make_unique<Graphics>(wnd.GetHWND());
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
    gfx->BeginFrame(0.2f, 0.4f, 0.9f);
    ImGui::ShowDemoWindow();
    gfx->EndFrame();
}
