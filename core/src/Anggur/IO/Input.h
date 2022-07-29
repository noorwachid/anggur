#pragma once

#include "Anggur/EventEmitter.h"
#include "InputEvent.h"
#include "WindowContext.h"
#include <array>

namespace Anggur {
    struct Keyboard {
        std::array<bool, maxKeyValue> currentState;
        std::array<bool, maxKeyValue> previousState; 

        void update();
    };

    struct Mouse {
        Vector2 currentPosition;
        Vector2 previousPosition;
        Vector2 direction;

        std::array<bool, maxMouseButtonValue> currentButtonState;
        std::array<bool, maxMouseButtonValue> previousButtonState;
        
        void update();
    };

    struct Scroll {
        Vector2 currentDirection;
        Vector2 previousDirection;

        void update();
    };

    class Input {
    public:
        // Key device

        bool isKeyPressed(Key key);
        
        bool isKeyReleased(Key key);
        
        bool IsKeyDown(Key key);
        
        bool isKeyUp(Key key);

        void setKeyState(Key key, bool state);


        // Mouse device

        bool isMouseButtonPressed(MouseButton button);
        
        bool isMouseButtonReleased(MouseButton button);
        
        bool isMouseButtonDown(MouseButton button);
        
        bool isMouseButtonUp(MouseButton button);

        const Vector2& getMousePosition();

        const Vector2& getMouseDirection();

        void setMouseButtonState(MouseButton button, bool state);

        void setMousePosition(const Vector2& position);

        // Scroll device

        const Vector2& getScrollDirection();

        void setScrollDirection(const Vector2& direction);
        
        // Data management

        void setContext(WindowContext* context);
        void update();

    private:
        Keyboard keyboard;
        Mouse mouse;
        Scroll scroll;

        EventEmitter emitter;

        WindowContext* context = nullptr;

    private:
        void directSetKeyState(Key key, bool state);

        void directSetMouseButtonState(MouseButton button, bool state);

        void directSetMousePosition(const Vector2& position);

        void directSetScrollDirection(const Vector2& direction);

        friend class Window;
    };
}