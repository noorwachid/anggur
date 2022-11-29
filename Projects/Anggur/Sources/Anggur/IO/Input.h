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
        virtual void update();
    };

    // Common Devices

    class Keyboard : public InputDevice
    {
      public:
        Keyboard(Window& window);

        void update() override;

        /**
         * Is key is just pressed
         */
        bool isKeyPressed(Key key);

        /**
         * Is key is just released
         */
        bool isKeyReleased(Key key);

        /**
         * Check if the key is down
         */
        bool isKeyDown(Key key);

        /**
         * Check if the key is up
         */
        bool isKeyUp(Key key);

        /**
         * Set the current key state
         */
        void setKeyState(Key key, bool state);

      private:
        /**
         * Set the current key state directly
         */
        void directSetKeyState(Key key, bool state);

      private:
        std::array<bool, maxKeyValue> currentState{false};
        std::array<bool, maxKeyValue> previousState{false};
    };

    enum class MouseCursorShape
    {
        arrow = 0x1,
        i = 0x2,
        crosshair = 0x3,
        hand = 0x4,
        horizontalResize = 0x5,
        verticalResize = 0x6,
    };

    class Mouse : public InputDevice
    {
      public:
        Mouse(Window& window);

        void update() override;

        /**
         * Is button is just pressed
         */
        bool isButtonPressed(MouseButton button);

        /**
         * Is button is just pressed
         */
        bool isButtonReleased(MouseButton button);

        /**
         * Check if the button is up
         */
        bool isButtonDown(MouseButton button);

        /**
         * Check if the button is up
         */
        bool isButtonUp(MouseButton button);

        /**
         * Get current cursor position
         */
        const Vector2& getCursorPosition();

        /**
         * Get cursor direction based on previous cursor position
         */
        const Vector2& getCursorDirection();

        /**
         * Get cursor cursor shape
         */
        MouseCursorShape getCursorShape();

        /**
         * Get wheel direction
         */
        const Vector2& getWheelDirection();

        /**
         * Set button state
         */
        void setButtonState(MouseButton button, bool state);

        /**
         * Set cursor position
         */
        void setCursorPosition(const Vector2& position);

        /**
         * Set cursor shape
         */
        void setCursorShape(MouseCursorShape mode);

        /**
         * Set wheel direction
         */
        void setWheelDirection(const Vector2& direction);

      private:
        /**
         * Set button state
         */
        void directSetButtonState(MouseButton button, bool state);

        /**
         * Set cursor position
         */
        void directSetCursorPosition(const Vector2& position);

        /**
         * Set wheel direction
         */
        void directSetWheelDirection(const Vector2& direction);

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