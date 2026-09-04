#include "Controller.h"

void Controller::Update(Window& wnd, Camera& cam, float dt)
{
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
        cam.Translate({0.0f, 0.0f, dt});
    }
    if (wnd.kbd.KeyIsPressed('A'))
    {
        cam.Translate({-dt, 0.0f, 0.0f});
    }
    if (wnd.kbd.KeyIsPressed('S'))
    {
        cam.Translate({0.0f, 0.0f, -dt});
    }
    if (wnd.kbd.KeyIsPressed('D'))
    {
        cam.Translate({dt, 0.0f, 0.0f});
    }
    if (wnd.kbd.KeyIsPressed('R'))
    {
        cam.Translate({0.0f, dt, 0.0f});
    }
    if (wnd.kbd.KeyIsPressed('F'))
    {
        cam.Translate({0.0f, -dt, 0.0f});
    }

    while (const auto delta = wnd.mouse.ReadRawDelta())
    {
        cam.Rotate((float)delta->x, (float)delta->y);
    }
}
