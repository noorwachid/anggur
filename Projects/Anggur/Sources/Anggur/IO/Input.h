#pragma once

#include "Anggur/EventEmitter.h"
#include "InputEvent.h"
#include <array>

namespace Anggur
{
    class Window;

    class InputDevice
    {
      protected:
        Window& window;

      public:
        InputDevice(Window& window);

        /**
         * Will be called on frame update
         */
        virtual void Update();
    };

    // Common Devices

    class Keyboard : public InputDevice
    {
      public:
        Keyboard(Window& window);

        void Update() override;

        /**
         * Is key is just pressed
         */
        bool IsKeyPressed(Key key);

        /**
         * Is key is just released
         */
        bool IsKeyReleased(Key key);

        /**
         * Check if the key is down
         */
        bool IsKeyDown(Key key);

        /**
         * Check if the key is up
         */
        bool IsKeyUp(Key key);

        /**
         * Set the current key state
         */
        void SetKeyState(Key key, bool state);

      private:
        /**
         * Set the current key state directly
         */
        void DirectSetKeyState(Key key, bool state);

      private:
        std::array<bool, maxKeyValue> currentState{false};
        std::array<bool, maxKeyValue> previousState{false};
    };

    enum class MouseCursorShape
    {
        Arrow = 0x1,
        I = 0x2,
        Crosshair = 0x3,
        Hand = 0x4,
        HorizontalResize = 0x5,
        VerticalResize = 0x6,
    };

    class Mouse : public InputDevice
    {
      public:
        Mouse(Window& window);

        void Update() override;

        /**
         * Is button is just pressed
         */
        bool IsButtonPressed(MouseButton button);

        /**
         * Is button is just pressed
         */
        bool IsButtonReleased(MouseButton button);

        /**
         * Check if the button is up
         */
        bool IsButtonDown(MouseButton button);

        /**
         * Check if the button is up
         */
        bool IsButtonUp(MouseButton button);

        /**
         * Get current cursor position
         */
        const Vector2& GetCursorPosition();

        /**
         * Get cursor direction based on previous cursor position
         */
        const Vector2& GetCursorDirection();

        /**
         * Get cursor cursor shape
         */
        MouseCursorShape GetCursorShape();

        /**
         * Get wheel direction
         */
        const Vector2& GetWheelDirection();

        /**
         * Set button state
         */
        void SetButtonState(MouseButton button, bool state);

        /**
         * Set cursor position
         */
        void SetCursorPosition(const Vector2& position);

        /**
         * Set cursor shape
         */
        void SetCursorShape(MouseCursorShape mode);

        /**
         * Set wheel direction
         */
        void SetWheelDirection(const Vector2& direction);

      private:
        /**
         * Set button state
         */
        void DirectSetButtonState(MouseButton button, bool state);

        /**
         * Set cursor position
         */
        void DirectSetCursorPosition(const Vector2& position);

        /**
         * Set wheel direction
         */
        void DirectSetWheelDirection(const Vector2& direction);

      private:
        Vector2 currentCursorPosition;
        Vector2 previousCursorPosition;
        Vector2 cursorDirection;

        std::array<bool, maxMouseButtonValue> currentButtonState{false};
        std::array<bool, maxMouseButtonValue> previousButtonState{false};

        MouseCursorShape shape;
        std::array<void*, 8> shapeBuffers{nullptr};

        Vector2 currentWheelDirection;
        Vector2 previousWheelDirection;
    };
}