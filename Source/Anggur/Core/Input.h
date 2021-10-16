#pragma once

#include <Anggur/Math/Vector.h>
#include "Application.h"
#include "Event/Event.h"

struct SDL_Window;

namespace Anggur {

class Input
{
public:
    static void Initialize();
    static void PreUpdate();
    static void Update();

    static bool IsKeyPressed(Scancode scancode);
    static bool IsKeyHeld(Scancode scancode);
    static bool IsKeyReleased(Scancode scancode);

    static bool IsKeyPressed(Key key);
    static bool IsKeyHeld(Key key);
    static bool IsKeyReleased(Key key);

    static bool IsMouseButtonPressed(MouseButton button);
    static bool IsMouseButtonHeld(MouseButton button);
    static bool IsMouseButtonReleased(MouseButton button);

    static void SetMouseCursor(const Vector& pos);

    static Vector GetMouseCursor();
    static Vector GetMouseWheel();

    friend class Application;

private:
    static SDL_Window* windowHandler;

    static const uint8_t* currScancodeMap;
    static uint8_t prevScancodeMap[Anggur_ScancodeLength];

    static uint currButtonMap;
    static uint prevButtonMap;
    static Vector cursor;
    static Vector wheel;
};

}

