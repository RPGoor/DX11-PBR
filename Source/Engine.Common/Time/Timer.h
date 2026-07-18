#pragma once
#include <chrono>

namespace Engine
{
    class Timer
    {
    public:
        Timer() noexcept;
        float Mark() noexcept;
        float Peek() const noexcept;
        float Elapsed() const noexcept;
    private:
        std::chrono::steady_clock::time_point last;
        std::chrono::steady_clock::time_point start;

    };
}

