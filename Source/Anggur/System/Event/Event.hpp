#pragma once

#include <Anggur/Helper/Primitive.hpp>
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

enum class EventGroup: uchar
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

struct KeyEvent : public Event
{
    int scancode;
    Key key;
    KeyMod mod;

    KeyEvent(EventType t, int sc, Key k, KeyMod m):
        Event(t, EventGroup::Key), 
	scancode(sc), 
	key(k), 
	mod(m) {}
};

struct MouseEvent : public Event
{
    Vector wheel;
    Vector pos;
    Vector dx;
    Mouse button;

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
