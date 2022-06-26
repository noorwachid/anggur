#pragma once

#include "InputEvent.h"
#include "Anggur/Event/EventManager.h"

namespace Anggur 
{
    class Input 
    {
    public:
        // Keyboard
        static bool IsKeyPressed(Key key);
        static bool IsKeyReleased(Key key);
        static bool IsKeyDown(Key key);
        static bool IsKeyUp(Key key);

        static void SetKeyState(Key key, bool state);

        // Mouse
        static bool IsMouseButtonPressed(MouseButton button);
        static bool IsMouseButtonReleased(MouseButton button);
        static bool IsMouseButtonDown(MouseButton button);
        static bool IsMouseButtonUp(MouseButton button);

        static void SetMouseButtonState(MouseButton button, bool state);

        static const Vector2& GetMousePosition();
        static const Vector2& GetMouseDirection();

        static void SetMousePosition(const Vector2& position);

        // Scroll
        static const Vector2& GetScrollDirection();

        static void SetScrollDirection(const Vector2& direction);

        static void Initialize();
        static void Update();

        static EventManager& GetEventManager();

    private:
        static void UpdateKeyboardData();
        static void UpdateMouseData();

        static void DirectSetKeyState(Key key, bool state);

        static void DirectSetMouseButtonState(MouseButton button, bool state);
        static void DirectSetMousePosition(const Vector2& position);

        static void DirectSetScrollDirection(const Vector2& direction);

        friend class Window;
    };
}