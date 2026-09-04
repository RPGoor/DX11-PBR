#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
    return keystates[keycode];
}

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
    if (keybuffer.size() > 0u)
    {
        Keyboard::Event e = keybuffer.front();
        keybuffer.pop();
        return e;
    }
    return {};
}

bool Keyboard::KeyIsEmpty() const noexcept
{
    return keybuffer.empty();
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    keystates[keycode] = true;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
    TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
    keystates[keycode] = false;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
    TrimBuffer(keybuffer);
}

void Keyboard::ClearState() noexcept
{
    keystates.reset();
}

template <typename T> void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}
