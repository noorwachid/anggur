#pragma once

#include "Anggur/EventEmitter.h"
#include "InputEvent.h"
#include <array>

namespace Anggur 
{
    class Window;

    class Input
    {
    protected:
        Window& window;

    public:
        Input(Window& window);

        virtual void Update();
    };

    // Common Devices

    class Keyboard: public Input
    {
    public:
        Keyboard(Window& window);
        void Update();

        bool IsKeyPressed(Key key);
        bool IsKeyReleased(Key key);
        bool IsKeyDown(Key key);
        bool IsKeyUp(Key key);

        void SetKeyState(Key key, bool state);

    private:
        void DirectSetKeyState(Key key, bool state);

    private:
        std::array<bool, maxKeyValue> currentState{false};
        std::array<bool, maxKeyValue> previousState{false}; 
    };

    class Mouse: public Input
    {
    public:
        Mouse(Window& window);
        void Update();

        bool IsButtonPressed(MouseButton button);
        bool IsButtonReleased(MouseButton button);
        bool IsButtonDown(MouseButton button);
        bool IsButtonUp(MouseButton button);

        const Vector2& GetPosition();
        const Vector2& GetDirection();

        void SetButtonState(MouseButton button, bool state);
        void SetPosition(const Vector2& position);

    private:
        void DirectSetButtonState(MouseButton button, bool state);
        void DirectSetPosition(const Vector2& position);
    
    private:
        Vector2 currentPosition;
        Vector2 previousPosition;
        Vector2 direction;

        std::array<bool, maxMouseButtonValue> currentButtonState{false};
        std::array<bool, maxMouseButtonValue> previousButtonState{false};
    };

    class Scroller: public Input
    {
    public:
        Scroller(Window& window);
        void Update();

        const Vector2& GetDirection();

        void SetDirection(const Vector2& direction);

    private:
        void DirectSetDirection(const Vector2& direction);

    private:
        Vector2 currentDirection;
        Vector2 previousDirection;
    };
}