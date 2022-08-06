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

    enum class MouseShape 
    {
        Arrow = 0x1,
        I = 0x2,
        Crosshair = 0x3,
        Hand = 0x4,
        HResize = 0x5,
        VResize = 0x6,
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

        MouseShape GetShape();

        void SetButtonState(MouseButton button, bool state);
        void SetPosition(const Vector2& position);
        void SetShape(MouseShape mode);

    private:
        void DirectSetButtonState(MouseButton button, bool state);
        void DirectSetPosition(const Vector2& position);
    
    private:
        Vector2 currentPosition;
        Vector2 previousPosition;
        Vector2 direction;

        std::array<bool, maxMouseButtonValue> currentButtonState{false};
        std::array<bool, maxMouseButtonValue> previousButtonState{false};

        MouseShape shape;
        std::array<void*, 8> shapeBuffers{nullptr};
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