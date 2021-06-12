#pragma once

#include <Anggur/Math/Vector.hpp>
#include "Event/Event.hpp"
#include "Core.hpp"

struct SDL_Window;

namespace Anggur {

class Input
{
public:
    static void Initialize();
    static void PreUpdate();
    static void Update();

    static bool IsPressed(Key key);
    static bool IsHeld(Key key);
    static bool IsReleased(Key key);

    static bool IsPressed(VirtKey vir);
    static bool IsHeld(VirtKey vir);
    static bool IsReleased(VirtKey vir);

    static bool IsPressed(MouseButton button);
    static bool IsHeld(MouseButton button);
    static bool IsReleased(MouseButton button);

    static bool IsWheelScrolled();

    static void SetCursorPos(const Vector& pos);
    static Vector GetCursorPos();
    static Vector GetWheel();

    friend class Application;

private:
    static SDL_Window* windowHandler;

    static const uchar* keyCurrState;
    static uchar keyPrevState[Anggur_ScancodeLength];

    static uint mouseCurrState;
    static uint mousePrevState;
    static Vector mousePos;
    static Vector mouseWheel;
};

}

