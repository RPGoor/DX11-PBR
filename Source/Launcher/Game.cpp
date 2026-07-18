#include "Game.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <GDIPlusManager.h>
#include <random>

GDIPlusManager gdipm;

Game::Game()
    : wnd(800, 600, L"Solorn Engine Window"), gfx(Graphics(wnd.GetHWND())), pointLight(gfx), frameBuffer(gfx)
{
    gfx.SetProjection(DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XMConvertToRadians(60.0f),
        16.0f / 9.0f,
        0.1f,
        100.0f
    ));

    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_real_distribution<float> positionDist(
        -10.0f,
        10.0f
    );

    std::uniform_real_distribution<float> rotationDist(
        0.0f,
        DirectX::XM_2PI
    );

    std::uniform_real_distribution<float> scaleDist(
        0.8f,
        1.2f
    );
    std::uniform_real_distribution<float> heightDist(
        0.3f,
        1.5f
    );

    std::uniform_real_distribution<float> colorDistR(
        0.75f,
        1.4f
    );
    std::uniform_real_distribution<float> colorDistG(
        0.8f,
        1.2f
    );
    std::uniform_real_distribution<float> colorDistB(
        0.85f,
        1.4f
    );

    std::vector<InstanceData> instances;

    for (int i = 0; i < 100000; ++i)
    {
        const float x = positionDist(rng);
        const float z = positionDist(rng);
        const float rotation = rotationDist(rng);
        const float scale = scaleDist(rng);

        const DirectX::XMMATRIX matrix =
            DirectX::XMMatrixScaling(
                scale,
                heightDist(rng),
                scale
            )
            *
            DirectX::XMMatrixRotationY(rotation)
            *
            DirectX::XMMatrixTranslation(
                x,
                0.25f,
                z
            );

        DirectX::XMFLOAT4X4 storedMatrix;
        DirectX::XMStoreFloat4x4(
            &storedMatrix,
            matrix
        );

        InstanceData instance{};

        instance.row0 = DirectX::XMFLOAT4(
            storedMatrix._11,
            storedMatrix._12,
            storedMatrix._13,
            storedMatrix._14
        );

        instance.row1 = DirectX::XMFLOAT4(
            storedMatrix._21,
            storedMatrix._22,
            storedMatrix._23,
            storedMatrix._24
        );

        instance.row2 = DirectX::XMFLOAT4(
            storedMatrix._31,
            storedMatrix._32,
            storedMatrix._33,
            storedMatrix._34
        );

        instance.row3 = DirectX::XMFLOAT4(
            storedMatrix._41,
            storedMatrix._42,
            storedMatrix._43,
            storedMatrix._44
        );

        instance.row4 = DirectX::XMFLOAT4(
            colorDistR(rng),
            colorDistG(rng),
            colorDistB(rng),
            1.0f
        );

        instances.push_back(instance);
    }

    grass = std::make_unique<InstancedModel>(
        gfx,
        "..\\..\\Assets\\Models\\grass.obj",
        instances,
        MaterialConstants{ {0.15f, 0.63f, 0.23f}, 0.1, 0.5, {} }
    );
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

    grid.Draw(gfx, DirectX::XMMatrixIdentity());

    grass->Draw(gfx, DirectX::XMMatrixIdentity());


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
    gfx.EndFrame();
}
