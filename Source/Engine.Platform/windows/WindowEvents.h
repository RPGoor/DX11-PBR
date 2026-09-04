#pragma once
#include <optional>

class WindowEvents
{
  public:
    struct ResizeEvent
    {
        unsigned int width;
        unsigned int height;
    };

    std::optional<ResizeEvent> GetEvent() noexcept;
    void SetEvent(unsigned int width, unsigned int height) noexcept;

  private:
    std::optional<ResizeEvent> pendingResize;
};
