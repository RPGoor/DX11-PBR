#pragma once 
#include "MinWindows.h"
#include <string>
#include <optional>

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


    //Graphics& Gfx();
    //Keyboard kbd;
    //Mouse mouse;
    //std::unique_ptr<Graphics> pGfx;
private:
    int width;
    int height;
    HWND hWnd;

    static LRESULT CALLBACK SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK PassMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

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