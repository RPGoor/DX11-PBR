#pragma once
#include <queue>
#include <optional>

class Mouse
{
    friend class Window;
public:
    struct RawDelta
    {
        int x, y;
    };
    class Event
    {
    public:
        enum class Type
        {
            Move,
            Enter,
            Leave,
        };
    private:
        Type type;
        bool leftIsPressed;
        bool rightIsPressed;
        int x;
        int y;
    public:
        Event(Type type, const Mouse& parent) noexcept
            :
            type(type),
            x(parent.x),
            y(parent.y)
        {}
        Type GetType() const noexcept
        {
            return type;
        }
        std::pair<int, int> GetPos() const noexcept
        {
            return{ x,y };
        }
        int GetPosX() const noexcept
        {
            return x;
        }
        int GetPosY() const noexcept
        {
            return y;
        }
        bool LeftIsPressed() const noexcept
        {
            return leftIsPressed;
        }
        bool RightIsPressed() const noexcept
        {
            return rightIsPressed;
        }
    };
public:
    Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    std::pair<int, int> GetPos() const noexcept;
    std::optional<RawDelta> ReadRawDelta() noexcept;
    bool IsInWindow() const noexcept;

    void EnableRaw() noexcept;
    void DisableRaw() noexcept;
    bool RawEnabled() const noexcept;
private:
    void OnMouseMove(int x, int y) noexcept;
    void OnMouseLeave() noexcept;
    void OnMouseEnter() noexcept;
    void OnRawDelta(int dx, int dy) noexcept;


    void TrimBuffer() noexcept;
    void TrimRawInputBuffer() noexcept;
private:
    static constexpr unsigned int bufferSize = 16u;
    int x = 0;
    int y = 0;
    bool isInWindow = false;
    bool rawEnabled = false;
    std::queue<Event> buffer;
    std::queue<RawDelta> rawDeltaBuffer;
};