#include <MinWindows.h>
#include <Window.h>

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    Window test = Window(800, 600, L"Asole Engine");
    return 0;
}