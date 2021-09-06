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

    static bool IsMouseButtonPressed(MouseButton button);
    static bool IsMouseButtonHeld(MouseButton button);
    static bool IsMouseButtonReleased(MouseButton button);

    static void SetMousePosition(const Vector& pos);

    static Vector GetMousePosition();
    static Vector GetMouseScrollDirection();

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

