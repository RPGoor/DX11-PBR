#include "WindowEvents.h"

std::optional<WindowEvents::ResizeEvent> WindowEvents::GetEvent() noexcept
{
    const auto event = pendingResize;
    pendingResize.reset();
    return event;
}

void WindowEvents::SetEvent(unsigned int width, unsigned int height) noexcept
{
    pendingResize = ResizeEvent{ width, height };
}
