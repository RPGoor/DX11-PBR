#include <MinWindows.h>
#include "Mouse.h"


std::pair<int, int> Mouse::GetPos() const noexcept
{
    return { x,y };
}

std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() noexcept
{
    if (rawDeltaBuffer.empty())
    {
        return std::nullopt;
    }
    const RawDelta d = rawDeltaBuffer.front();
    rawDeltaBuffer.pop();
    return d;
}

bool Mouse::IsInWindow() const noexcept
{
    return isInWindow;
}

void Mouse::EnableRaw() noexcept
{
    rawEnabled = true;
}

void Mouse::DisableRaw() noexcept
{
    rawEnabled = false;
}

bool Mouse::RawEnabled() const noexcept
{
    return rawEnabled;
}

void Mouse::OnMouseMove(int newx, int newy) noexcept
{
    x = newx;
    y = newy;

    buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
    isInWindow = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
    isInWindow = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}

void Mouse::OnRawDelta(int dx, int dy) noexcept
{
    rawDeltaBuffer.push({ dx,dy });
    TrimRawInputBuffer();
}

void Mouse::TrimBuffer() noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}

void Mouse::TrimRawInputBuffer() noexcept
{
    while (rawDeltaBuffer.size() > bufferSize)
    {
        rawDeltaBuffer.pop();
    }
}
