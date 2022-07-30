#pragma once

#include "Anggur/EventEmitter.h"
#include "InputEvent.h"
#include "WindowContext.h"
#include <array>

namespace Anggur 
{
    struct Keyboard 
    {
        std::array<bool, maxKeyValue> currentState{false};
        std::array<bool, maxKeyValue> previousState{false}; 

        void Update();
    };

    struct Mouse 
    {
        Vector2 currentPosition;
        Vector2 previousPosition;
        Vector2 direction;

        std::array<bool, maxMouseButtonValue> currentButtonState{false};
        std::array<bool, maxMouseButtonValue> previousButtonState{false};
        
        void Update();
    };

    struct Scroll 
    {
        Vector2 currentDirection;
        Vector2 previousDirection;

        void Update();
    };

    class Input 
    {
    public:
        // Key device

        bool IsKeyPressed(Key key);
        
        bool IsKeyReleased(Key key);
        
        bool IsKeyDown(Key key);
        
        bool IsKeyUp(Key key);

        void SetKeyState(Key key, bool state);


        // Mouse device

        bool IsMouseButtonPressed(MouseButton button);
        
        bool IsMouseButtonReleased(MouseButton button);
        
        bool IsMouseButtonDown(MouseButton button);
        
        bool IsMouseButtonUp(MouseButton button);

        const Vector2& GetMousePosition();

        const Vector2& GetMouseDirection();

        void SetMouseButtonState(MouseButton button, bool state);

        void SetMousePosition(const Vector2& position);

        // Scroll device

        const Vector2& GetScrollDirection();

        void SetScrollDirection(const Vector2& direction);
        
        // Data management

        void SetContext(WindowContext* context);
        void Update();

    private:
        Keyboard keyboard;
        Mouse mouse;
        Scroll scroll;

        EventEmitter emitter;

        WindowContext* context = nullptr;

    private:
        void DirectSetKeyState(Key key, bool state);

        void DirectSetMouseButtonState(MouseButton button, bool state);

        void DirectSetMousePosition(const Vector2& position);

        void DirectSetScrollDirection(const Vector2& direction);

        friend class Window;
    };
}