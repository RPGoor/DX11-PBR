#include "Game.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <random>

Game::Game()
    : wnd(1280, 720, L"Solorn Engine Window"), gfx(Graphics(wnd.GetHWND())), frameBuffer(gfx), scene(gfx)
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

    if (const auto resize = wnd.events.GetEvent())
    {
        gfx.Resize(
            resize->width,
            resize->height
        );

    }
    Camera& cam = scene.GetCamera();
    controller.Update(wnd, cam, dt);

    scene.Update(dt);

    gfx.BeginFrame(0.72f, 0.80f, 0.84f);

    frameBuffer.Update(
        gfx,
        cam.GetMatrix(),
        cam.GetProjection(),
        cam.pos,
        timer.Elapsed()
    );

    scene.Draw(gfx);

    gfx.EndFrame();
}
