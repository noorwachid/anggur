#include "Input.h"
#include "Internal.h"
#include "Window.h"
#include <array>
#include <iostream>

namespace Anggur
{
    // Input

    InputDevice::InputDevice(Window &newWindow) : window(newWindow)
    {
        window.registerInputDevice(this);
    }

    void InputDevice::update()
    {
    }

    // Keyboard

    Keyboard::Keyboard(Window &window) : InputDevice(window)
    {
        glfwSetKeyCallback(window.getContext(), [](GLFWwindow *context, int vkeyCode, int scanCode, int action,
                                                   int modifierKey) {
            auto window = static_cast<Window *>(glfwGetWindowUserPointer(context));
            Key key = static_cast<Key>(vkeyCode);
            window->getInputDevice<Keyboard>().directSetKeyState(key, action == GLFW_PRESS || action == GLFW_REPEAT);
        });
    }

    void Keyboard::update()
    {
        for (int i = 0; i < previousState.size(); ++i)
            if (previousState[i] != currentState[i])
                previousState[i] = currentState[i];
    }

    bool Keyboard::isKeyPressed(Key key)
    {
        int index = static_cast<int>(key);
        return currentState[index] && previousState[index] == false;
    }

    bool Keyboard::isKeyReleased(Key key)
    {
        int index = static_cast<int>(key);
        return currentState[index] == false && previousState[index];
    }

    bool Keyboard::isKeyDown(Key key)
    {
        int index = static_cast<int>(key);
        return currentState[index];
    }

    bool Keyboard::isKeyUp(Key key)
    {
        int index = static_cast<int>(key);
        return currentState[index] == false;
    }

    void Keyboard::setKeyState(Key key, bool state)
    {
        directSetKeyState(key, state);
    }

    void Keyboard::directSetKeyState(Key key, bool state)
    {
        int index = static_cast<int>(key);
        currentState[index] = state;
        KeyEvent event(state ? "KeyPressed" : "KeyReleased", key);
        window.emitter.Dispatch(event);
    }

    // Input mouse device

    Mouse::Mouse(Window &window) : InputDevice(window)
    {
        glfwSetMouseButtonCallback(
            window.getContext(), [](GLFWwindow *context, int button, int action, int modidefrKey) {
                auto window = static_cast<Window *>(glfwGetWindowUserPointer(context));
                MouseButton b = static_cast<MouseButton>(button);
                window->getInputDevice<Mouse>().directSetButtonState(b, action == GLFW_PRESS);
            });

        glfwSetCursorPosCallback(window.getContext(), [](GLFWwindow *context, double x, double y) {
            auto window = static_cast<Window *>(glfwGetWindowUserPointer(context));
            window->getInputDevice<Mouse>().directSetCursorPosition(Vector2(x, y));
        });

        glfwSetScrollCallback(window.getContext(), [](GLFWwindow *context, double x, double y) {
            auto window = static_cast<Window *>(glfwGetWindowUserPointer(context));
            window->getInputDevice<Mouse>().directSetWheelDirection(Vector2(x, y));
        });
    }

    void Mouse::update()
    {
        for (int i = 0; i < previousButtonState.size(); ++i)
            if (previousButtonState[i] != currentButtonState[i])
                previousButtonState[i] = currentButtonState[i];

        cursorDirection = currentCursorPosition - previousCursorPosition;
        previousCursorPosition = currentCursorPosition;

        previousWheelDirection = currentWheelDirection;
        currentWheelDirection.set(0.0f, 0.0f);
    }

    bool Mouse::isButtonPressed(MouseButton button)
    {
        int index = static_cast<int>(button);
        return currentButtonState[index] && previousButtonState[index] == false;
    }

    bool Mouse::isButtonReleased(MouseButton button)
    {
        int index = static_cast<int>(button);
        return currentButtonState[index] == false && previousButtonState[index];
    }

    bool Mouse::isButtonDown(MouseButton button)
    {
        int index = static_cast<int>(button);
        return currentButtonState[index];
    }

    bool Mouse::isButtonUp(MouseButton button)
    {
        int index = static_cast<int>(button);
        return currentButtonState[index] == false;
    }

    const Vector2 &Mouse::getCursorPosition()
    {
        return currentCursorPosition;
    }

    const Vector2 &Mouse::getCursorDirection()
    {
        return cursorDirection;
    }

    MouseCursorShape Mouse::getCursorShape()
    {
        return shape;
    }

    const Vector2 &Mouse::getWheelDirection()
    {
        return currentWheelDirection;
    }

    void Mouse::setButtonState(MouseButton button, bool state)
    {
        directSetButtonState(button, state);
    }

    void Mouse::setCursorShape(MouseCursorShape newShape)
    {
        int intMode = static_cast<int>(newShape);
        if (shapeBuffers[intMode] == nullptr)
        {
            shapeBuffers[intMode] = glfwCreateStandardCursor(intMode + 0x00036000);
        }
        glfwSetCursor(window.getContext(), static_cast<GLFWcursor *>(shapeBuffers[intMode]));
        shape = newShape;
    }

    void Mouse::setCursorPosition(const Vector2 &position)
    {
        // Not calling direct function because it modifies the data differently

        glfwSetCursorPos(window.getContext(), position.x, position.y);
        currentCursorPosition = position;
        MousePositionEvent event("MousePositionMove", position);
        window.emitter.Dispatch(event);
    }

    void Mouse::setWheelDirection(const Vector2 &direction)
    {
        directSetWheelDirection(direction);
    }

    void Mouse::directSetButtonState(MouseButton button, bool state)
    {
        currentButtonState[static_cast<int>(button)] = state;
        MouseButtonEvent event(state ? "MouseButtonPressed" : "MouseButtonReleased", button);
        window.emitter.Dispatch(event);
    }

    void Mouse::directSetCursorPosition(const Vector2 &position)
    {
        currentCursorPosition = position;
        MousePositionEvent event("MousePositionMoved", position);
        window.emitter.Dispatch(event);
    }

    void Mouse::directSetWheelDirection(const Vector2 &direction)
    {
        currentWheelDirection = direction;
        ScrollEvent event("ScrollMoved", direction);
        window.emitter.Dispatch(event);
    }
}