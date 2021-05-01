#pragma once

#include <Anggur/Math/Vector.hpp>
#include "Event/Event.hpp"
#include "Core.hpp"

namespace Anggur {

class Input
{
public:
    static void Initialize();
    static void Update();

    static bool IsKeyPressed(Key key);
    static bool IsKeyHeld(Key key);
    static bool IsKeyReleased(Key key);

    static bool IsScancodePressed(int i);
    static bool IsScancodeHeld(int i);
    static bool IsScancodeReleased(int i);

    static void SetMousePos(const Vector& pos);
    static const Vector GetMousePos();
    static const Vector GetMouseWheel();

    friend class Application;

private:
    static const Uint8* mCurrKeyState;
    static Uint8 mPrevKeyState[SDL_NUM_SCANCODES];
};

}

