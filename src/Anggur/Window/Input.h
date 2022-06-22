#pragma once

#include "Key.h"

namespace Anggur 
{
    class Input 
    {
    public:
        static bool IsKeyPressed(Key key);
        static bool IsKeyReleased(Key key);
        static bool IsKeyDown(Key key);
        static bool IsKeyUp(Key key);

    private:
        static void Update();
        static void UpdateKeyState();
        static void SetKeyState(Key key, bool state);

        friend class Window;
    };
}