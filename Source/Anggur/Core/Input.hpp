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

    static bool IsKeyPressed(Scancode scancode);
    static bool IsKeyHeld(Scancode scancode);
    static bool IsKeyReleased(Scancode scancode);

    static bool IsKeyPressed(Key key);
    static bool IsKeyHeld(Key key);
    static bool IsKeyReleased(Key key);

    static bool IsButtonPressed(Button button);
    static bool IsButtonHeld(Button button);
    static bool IsButtonReleased(Button button);

    static void SetCursor(const Vector& pos);

    static Vector GetCursor();
    static Vector GetWheel();

    friend class Core;

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

