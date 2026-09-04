#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Window.h"
#include "WindowExceptionsMacros.h"

#define IDI_ENGINE_ICON 101

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::WindowClass Window::WindowClass::wndClass;

Window::Window(int width, int height, LPCWSTR name)
    : width(width),
      height(height)
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if (!(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
    {
        throw WND_LAST_EXCEPT();
    }

    hWnd = CreateWindow(
        WindowClass::GetName(),
        name,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        WindowClass::GetInstance(),
        this
    );

    if (hWnd == nullptr)
    {
        throw WND_LAST_EXCEPT();
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);

    ImGui_ImplWin32_Init(hWnd);

    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01; // mouse page
    rid.usUsage = 0x02; // mouse usage
    rid.dwFlags = 0;
    rid.hwndTarget = nullptr;
    if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
    {
        throw WND_LAST_EXCEPT();
    }
}

Window::~Window()
{
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string& title)
{
    if (SetWindowTextA(hWnd, title.c_str()) == 0)
    {
        throw WND_LAST_EXCEPT();
    }
}

std::optional<int> Window::ProcessMessagePump()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return (int)msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

HWND Window::GetHWND() const
{
    return hWnd;
}

LRESULT Window::SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::PassMessageHandler));
        return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::EnableCursor() noexcept
{
    cursorEnabled = true;
    ShowCursor();
    EnableImGuiMouse();
    FreeCursor();
}

void Window::DisableCursor() noexcept
{
    cursorEnabled = false;
    HideCursor();
    DisableImGuiMouse();
    ConfineCursor();
}

bool Window::CursorEnabled() const noexcept
{
    return cursorEnabled;
}

LRESULT Window::PassMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }
    const ImGuiIO imGuiIO = ImGui::GetIO();

    switch (msg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_KILLFOCUS:
            kbd.ClearState();
            break;
        case WM_ACTIVATE:
            if (!cursorEnabled)
            {
                if (wParam & WA_ACTIVE)
                {
                    ConfineCursor();
                    HideCursor();
                }
                else
                {
                    FreeCursor();
                    ShowCursor();
                }
            }
            break;
        case WM_ENTERSIZEMOVE:
            isBeingResized = true;
            return 0;
        case WM_SIZE:
        {
            const auto newWidth = LOWORD(lParam);
            const auto newHeight = HIWORD(lParam);
            if (width == newWidth && height == newHeight)
            {
                return 0;
            }

            if (width > 0 && height > 0)
            {
                width = newWidth;
                height = newHeight;
                events.SetEvent(width, height);
            }
            return 0;
        }
        case WM_EXITSIZEMOVE:
            isBeingResized = false;
            return 0;
            /*********** KEYBOARD MESSAGES ***********/
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (imGuiIO.WantCaptureKeyboard)
            {
                break;
            }
            if (!(lParam & 0x40000000))
            {
                kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (imGuiIO.WantCaptureKeyboard)
            {
                break;
            }
            kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
            break;
            /*********** END KEYBOARD MESSAGES ***********/

            /************* MOUSE MESSAGES ****************/
        case WM_MOUSEMOVE:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            if (!cursorEnabled)
            {
                if (!mouse.IsInWindow())
                {
                    SetCapture(hWnd);
                    mouse.OnMouseEnter();
                    HideCursor();
                }
                break;
            }
            if (imGuiIO.WantCaptureMouse)
            {
                break;
            }
            if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
            {
                mouse.OnMouseMove(pt.x, pt.y);
                if (!mouse.IsInWindow())
                {
                    SetCapture(hWnd);
                    mouse.OnMouseEnter();
                }
            }
            else
            {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            SetForegroundWindow(hWnd);
            if (!cursorEnabled)
            {
                ConfineCursor();
                HideCursor();
            }
            if (imGuiIO.WantCaptureMouse)
            {
                break;
            }
            const POINTS pt = MAKEPOINTS(lParam);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            if (imGuiIO.WantCaptureMouse)
            {
                break;
            }
            const POINTS pt = MAKEPOINTS(lParam);
            break;
        }
        case WM_LBUTTONUP:
        {
            if (imGuiIO.WantCaptureMouse)
            {
                break;
            }
            const POINTS pt = MAKEPOINTS(lParam);
            if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
            {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
            break;
        }
        case WM_RBUTTONUP:
        {
            if (imGuiIO.WantCaptureMouse)
            {
                break;
            }
            const POINTS pt = MAKEPOINTS(lParam);
            if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
            {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
            break;
        }
        /************** END MOUSE MESSAGES **************/

        /************** RAW MOUSE MESSAGES **************/
        case WM_INPUT:
        {
            if (!mouse.RawEnabled())
            {
                break;
            }
            UINT size = 0;
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER))
                == -1)
            {
                break;
            }
            rawBuffer.resize(size);
            if (GetRawInputData(
                    reinterpret_cast<HRAWINPUT>(lParam),
                    RID_INPUT,
                    rawBuffer.data(),
                    &size,
                    sizeof(RAWINPUTHEADER)
                )
                != size)
            {
                break;
            }
            auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
            if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
            {
                mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
            }
            break;
        }
            /************** END RAW MOUSE MESSAGES **************/
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::ConfineCursor() noexcept
{
    RECT rect;
    GetClientRect(hWnd, &rect);
    MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
    ClipCursor(&rect);
}

void Window::FreeCursor() noexcept
{
    ClipCursor(nullptr);
}

void Window::HideCursor() noexcept
{
    while (::ShowCursor(FALSE) >= 0)
        ;
}

void Window::ShowCursor() noexcept
{
    while (::ShowCursor(TRUE) < 0)
        ;
}

void Window::EnableImGuiMouse() noexcept
{
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouse() noexcept
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

#pragma region Window Class

Window::WindowClass::WindowClass() noexcept
    : hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = SetupMessageHandler;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ENGINE_ICON), IMAGE_ICON, 32, 32, 0));
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ENGINE_ICON), IMAGE_ICON, 16, 16, 0));
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}

LPCWSTR Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

#pragma endregion
