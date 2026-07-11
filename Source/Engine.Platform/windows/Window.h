#pragma once 
#include <MinWindows.h>
#include <string>
#include <optional>
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"

class Window
{
public:
    Window(int width, int height, LPCWSTR name);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

public:
    void SetTitle(const std::string& title);
    static std::optional<int> ProcessMessagePump();
    HWND GetHWND() const;
    void EnableCursor() noexcept;
    void DisableCursor() noexcept;
    bool CursorEnabled() const noexcept;

public:
    Mouse mouse;
    Keyboard kbd;

private:
    int width;
    int height;
    HWND hWnd;
    bool cursorEnabled = true;
    std::vector<BYTE> rawBuffer;

    static LRESULT CALLBACK SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK PassMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    void ConfineCursor() noexcept;
    void FreeCursor() noexcept;
    void ShowCursor() noexcept;
    void HideCursor() noexcept;
    void EnableImGuiMouse() noexcept;
    void DisableImGuiMouse() noexcept;
private:
    class WindowClass
    {
    private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;

    public:
        static LPCWSTR GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;

    private:
        static constexpr const wchar_t* wndClassName = L"Solorn Engine Window";
        static WindowClass wndClass;
        HINSTANCE hInst;
    };
};