#pragma once

#include <Anggur/Math/Vector.hpp>
#include "Window.hpp"
#include "Key.hpp"
#include "Mouse.hpp"
#include "Gamepad.hpp"

namespace Anggur {

enum class EventType
{
    WindowClosed,
    WindowResized,
    WindowMoved,

    MouseMoved,
    MouseScrolled,
    MousePressed,
    MouseHeld,
    MouseReleased,

    KeyPressed,
    KeyHeld,
    KeyReleased,
    KeyInput,

    GamepadConnected,
    GamepadDisconnected,
    GamepadAxis,
};

enum class EventGroup: uint8_t
{
    Window  = 0x1,
    Mouse   = 0x2,
    Key     = 0x4,
    Gamepad = 0x8,
};

struct Event
{
    EventType type;
    EventGroup group;
    bool handled;

    Event(EventType t, EventGroup g): type(t), group(g), handled(false) {};
};

struct WindowEvent : public Event
{
    Vector pos;
    Vector size;

    WindowEvent(EventType t):
        Event(t, EventGroup::Window)
    {}
};

struct KeyboardEvent : public Event
{
    Scancode scancode;
    Key key;
    Modifier modifier;

    KeyboardEvent(EventType t, Scancode c, Key k, Modifier m):
        Event(t, EventGroup::Key), 
        scancode(c),
        key(k),
        modifier(m) {}
};

struct MouseEvent : public Event
{
    Vector wheel;
    Vector cursor;
    Vector dx;
    Button button;

    MouseEvent(EventType t):
        Event(t, EventGroup::Mouse)
    {}
};

// struct GamepadEvent : public Event
// {
//     Gamepad button;
//     GamepadState state;
//     GamepadAxis axis;
// };

}
