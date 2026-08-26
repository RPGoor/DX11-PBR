#include "Game.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <random>

Game::Game()
    : wnd(1280, 720, L"Solorn Engine Window"), gfx(Graphics(wnd.GetHWND())), pointLight(gfx), frameBuffer(gfx), landscape(gfx)
{
    gfx.SetProjection(DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XMConvertToRadians(60.0f),
        16.0f / 9.0f,
        0.1f,
        1000.0f
    ));

    landscape.GenerateTerrain(gfx);
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
    if (const auto resize = wnd.events.GetEvent())
    {
        gfx.Resize(
            resize->width,
            resize->height
        );

    }

    const auto dt = timer.Mark();
    const auto view = cam.GetMatrix();
    const auto projection = gfx.GetProjection();

    float test = timer.Elapsed();

    frameBuffer.Update(
        gfx,
        view,
        projection,
        cam.pos,
        timer.Elapsed()
    );
    frameBuffer.Bind(gfx);
    pointLight.Bind(gfx, cam.GetMatrix());

    gfx.BeginFrame(0.2f, 0.4f, 0.9f);
    gfx.SetCamera(cam.GetMatrix());

    landscape.Draw(gfx);

    while (const auto e = wnd.kbd.ReadKey())
    {
        if (!e->IsPress())
        {
            continue;
        }

        switch (e->GetCode())
        {
        case VK_ESCAPE:
            if (wnd.CursorEnabled())
            {
                wnd.DisableCursor();
                wnd.mouse.EnableRaw();
            }
            else
            {
                wnd.EnableCursor();
                wnd.mouse.DisableRaw();
            }
            break;
        }
    }

    if (wnd.kbd.KeyIsPressed('W'))
    {
        cam.Translate({ 0.0f,0.0f,dt });
    }
    if (wnd.kbd.KeyIsPressed('A'))
    {
        cam.Translate({ -dt,0.0f,0.0f });
    }
    if (wnd.kbd.KeyIsPressed('S'))
    {
        cam.Translate({ 0.0f,0.0f,-dt });
    }
    if (wnd.kbd.KeyIsPressed('D'))
    {
        cam.Translate({ dt,0.0f,0.0f });
    }
    if (wnd.kbd.KeyIsPressed('R'))
    {
        cam.Translate({ 0.0f,dt,0.0f });
    }
    if (wnd.kbd.KeyIsPressed('F'))
    {
        cam.Translate({ 0.0f,-dt,0.0f });
    }

    while (const auto delta = wnd.mouse.ReadRawDelta())
    {
        
        cam.Rotate((float)delta->x, (float)delta->y);
        
    }

    landscape.SpawnControlWindows();

    gfx.EndFrame();
}
