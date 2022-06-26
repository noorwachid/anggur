#include "Input.h"
#include "Internal.h"
#include "Window.h"
#include <array>
#include <iostream>

namespace Anggur 
{
    struct KeyboardData 
    {
        std::array<bool, maxKeyValue> currentState;
        std::array<bool, maxKeyValue> previousState; 
    };

    struct MouseData 
    {
        Vector2 currentPosition;
        Vector2 previousPosition;
        Vector2 direction;

        std::array<bool, maxMouseButtonValue> currentButtonState;
        std::array<bool, maxMouseButtonValue> previousButtonState;
    };

    struct ScrollData
    {
        Vector2 direction;
    };

    KeyboardData keyboardData;
    MouseData mouseData;
    ScrollData scrollData;

    GLFWwindow* handler = nullptr;
    EventManager eventManager;

    // Keyboard Device

    bool Input::IsKeyPressed(Key key)
    {
        int index = static_cast<int>(key);
        return keyboardData.currentState[index] && keyboardData.previousState[index] == false;
    }

    bool Input::IsKeyReleased(Key key)
    {
        int index = static_cast<int>(key);
        return keyboardData.currentState[index] == false && keyboardData.previousState[index];
    }

    bool Input::IsKeyDown(Key key)
    {
        int index = static_cast<int>(key);
        return keyboardData.currentState[index];
    }

    bool Input::IsKeyUp(Key key)
    {
        int index = static_cast<int>(key);
        return keyboardData.currentState[index] == false;   
    }

    void Input::SetKeyState(Key key, bool state)
    {
        DirectSetKeyState(key, state);
    }

    // Mouse Device

    bool Input::IsMouseButtonPressed(MouseButton button)
    {
        int index = static_cast<int>(button);
        return mouseData.currentButtonState[index] && mouseData.previousButtonState[index] == false;
    }

    bool Input::IsMouseButtonReleased(MouseButton button)
    {
        int index = static_cast<int>(button);
        return mouseData.currentButtonState[index] == false && mouseData.previousButtonState[index];
    }

    bool Input::IsMouseButtonDown(MouseButton button)
    {
        int index = static_cast<int>(button);
        return mouseData.currentButtonState[index];
    }

    bool Input::IsMouseButtonUp(MouseButton button)
    {
        int index = static_cast<int>(button);
        return mouseData.currentButtonState[index] == false;
    }

    void Input::SetMouseButtonState(MouseButton button, bool state)
    {
        DirectSetMouseButtonState(button, state);
    }

    const Vector2& Input::GetMousePosition()
    {
        return mouseData.currentPosition;
    }

    const Vector2& Input::GetMouseDirection()
    {
        return mouseData.direction;
    }

    void Input::SetMousePosition(const Vector2& position)
    {
        // Not calling direct function because it modifies the data differently

        glfwSetCursorPos(handler, position.x, position.y);
        mouseData.currentPosition = position;
        MousePositionEvent event("MousePositionMove", position);
        eventManager.Emit(event);
    }

    // Scroll Virtual Device

    const Vector2& Input::GetScrollDirection()
    {
        return scrollData.direction;
    }

    void Input::SetScrollDirection(const Vector2& direction)
    {
        DirectSetScrollDirection(direction);
    }

    void Input::Initialize()
    {
        handler = static_cast<GLFWwindow*>(Window::GetHandler());

		glfwSetKeyCallback(handler, [](GLFWwindow* handler, int vkeyCode, int scanCode, int action, int modidefrKey) 
		{
			Key key = static_cast<Key>(vkeyCode);
            Input::DirectSetKeyState(key, action == GLFW_PRESS || action == GLFW_REPEAT);
		});


		glfwSetMouseButtonCallback(handler, [](GLFWwindow* handler, int button, int action, int modidefrKey) 
		{
			MouseButton b = static_cast<MouseButton>(button);
            Input::DirectSetMouseButtonState(b, action == GLFW_PRESS);
		});

		glfwSetCursorPosCallback(handler, [](GLFWwindow* handler, double x, double y)
		{
			Input::DirectSetMousePosition(Vector2(x, y));
		});
    }

    void Input::Update()
    {
        UpdateKeyboardData();
        UpdateMouseData();
    }

    EventManager& Input::GetEventManager()
    {
        return eventManager;
    }

    // Private Implementations

    void Input::UpdateKeyboardData()
    {
        for (int i = 0; i < keyboardData.previousState.size(); ++i) 
        {
            keyboardData.previousState[i] = keyboardData.currentState[i];
        }
    }

    void Input::UpdateMouseData()
    {
        for (int i = 0; i < mouseData.previousButtonState.size(); ++i) 
        {
            mouseData.previousButtonState[i] = mouseData.currentButtonState[i];
        }

        mouseData.direction = mouseData.currentPosition - mouseData.previousPosition;
        mouseData.previousPosition = mouseData.currentPosition;
    }

    void Input::DirectSetKeyState(Key key, bool state)
    {
        int index = static_cast<int>(key);
        keyboardData.currentState[index] = state;
        KeyEvent event(state ? "KeyPressed" : "KeyRelease", key);
        eventManager.Emit(event);
    }


    void Input::DirectSetMouseButtonState(MouseButton button, bool state)
    {
        mouseData.currentButtonState[static_cast<int>(button)] = state;
        MouseButtonEvent event(state ? "MouseButtonPressed" : "MouseButtonRelease", button);
        eventManager.Emit(event);
    }

    void Input::DirectSetMousePosition(const Vector2& position)
    {
        mouseData.currentPosition = position;
        MousePositionEvent event("MousePositionMove", position);
        eventManager.Emit(event);
    }

    void Input::DirectSetScrollDirection(const Vector2& direction)
    {
        scrollData.direction = direction;
        ScrollEvent event("ScrollMove", direction);
        eventManager.Emit(event);
    }
}