#include "Game.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <GDIPlusManager.h>
#include <random>

GDIPlusManager gdipm;

Game::Game()
    : wnd(800, 600, L"Solorn Engine Window"), gfx(Graphics(wnd.GetHWND())), pointLight(gfx)
{
    gfx.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> dist(-10.0f, 10.0f);

    grassTransforms.reserve(5000);

    for (int i = 0; i < 5000; ++i)
    {
        const auto matrix = DirectX::XMMatrixTranslation(
            dist(rng),
            0.2f,
            dist(rng)
        );

        DirectX::XMFLOAT4X4 storedMatrix;
        DirectX::XMStoreFloat4x4(&storedMatrix, matrix);

        grassTransforms.push_back(storedMatrix);
    }
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
    gfx.BeginFrame(0.2f, 0.4f, 0.9f);
    gfx.SetCamera(cam.GetMatrix());
    pointLight.Bind(gfx, cam.GetMatrix());

    grid.Draw(gfx, DirectX::XMMatrixIdentity());

    for (auto& g : grassTransforms)
    {
        grass.Draw(gfx, DirectX::XMLoadFloat4x4(&g));
    }

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

    pointLight.SpawnControlWindow();
    ImGui::ShowDemoWindow();
    gfx.EndFrame();
}
