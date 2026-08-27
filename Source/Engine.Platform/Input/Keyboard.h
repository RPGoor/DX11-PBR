#pragma once
#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
    friend class Window;
public:
    class Event
    {
    public:
        enum class Type
        {
            Press,
            Release,
        };
    private:
        Type type;
        unsigned char code;
    public:
        Event(Type type, unsigned char code) noexcept
            :
            type(type),
            code(code)
        {}
        bool IsPress() const noexcept
        {
            return type == Type::Press;
        }
        bool IsRelease() const noexcept
        {
            return type == Type::Release;
        }
        unsigned char GetCode() const noexcept
        {
            return code;
        }
    };
public:
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    bool KeyIsPressed(unsigned char keycode) const noexcept;
    std::optional<Event> ReadKey() noexcept;
    bool KeyIsEmpty() const noexcept;

private:
    void OnKeyPressed(unsigned char keycode) noexcept;
    void OnKeyReleased(unsigned char keycode) noexcept;
    void ClearState() noexcept;
    template<typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;
    std::bitset<nKeys> keystates;
    std::queue<Event> keybuffer;
};