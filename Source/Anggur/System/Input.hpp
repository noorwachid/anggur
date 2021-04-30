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

    static bool IsPressed(Key key);
    static bool IsHeld(Key key);
    static bool IsReleased(Key key);

    static bool IsPressed(Mouse button);
    static bool IsHeld(Mouse button);
    static bool IsReleased(Mouse button);

    static void SetCursorPos(const Vector& pos);
    static const Vector GetMousePos();
    static const Vector GetMouseWheel();

    friend class Application;

private:
};

}

