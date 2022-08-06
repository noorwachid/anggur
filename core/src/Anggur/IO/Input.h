#pragma once

#include "Anggur/EventEmitter.h"
#include "InputEvent.h"
#include "WindowContext.h"
#include <array>

namespace Anggur 
{
    class InputSystem;

    class Input
    {
    protected:
        InputSystem* system;

    public:
        Input(InputSystem* system);

        virtual void Update();
    };

    class InputSystem 
    {
    public:
        WindowContext* context = nullptr;
        std::vector<Input*> references;
        EventEmitter emitter;

        template <class Input>
        Input& Get() 
        {
            static Input input(this);

            return input;
        }

        // Data management

        void SetContext(WindowContext* context);
        void Update();

    private:
        friend class Window;
    };

    // Common Devices

    class Keyboard: public Input
    {
    public:
        Keyboard(InputSystem* system);
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
        Mouse(InputSystem* system);
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
        Scroller(InputSystem* system);
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