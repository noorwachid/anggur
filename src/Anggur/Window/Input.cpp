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

    struct InputData 
    {
        KeyboardData keyboard;
        MouseData mouse;
        ScrollData scroll;

        GLFWwindow* handler = nullptr;
        EventManager eventManager;
    };

    InputData inputData;

    // Keyboard Device

    bool Input::IsKeyPressed(Key key)
    {
        int index = static_cast<int>(key);
        return inputData.keyboard.currentState[index] && inputData.keyboard.previousState[index] == false;
    }

    bool Input::IsKeyReleased(Key key)
    {
        int index = static_cast<int>(key);
        return inputData.keyboard.currentState[index] == false && inputData.keyboard.previousState[index];
    }

    bool Input::IsKeyDown(Key key)
    {
        int index = static_cast<int>(key);
        return inputData.keyboard.currentState[index];
    }

    bool Input::IsKeyUp(Key key)
    {
        int index = static_cast<int>(key);
        return inputData.keyboard.currentState[index] == false;   
    }

    void Input::SetKeyState(Key key, bool state)
    {
        DirectSetKeyState(key, state);
    }

    // Mouse Device

    bool Input::IsMouseButtonPressed(MouseButton button)
    {
        int index = static_cast<int>(button);
        return inputData.mouse.currentButtonState[index] && inputData.mouse.previousButtonState[index] == false;
    }

    bool Input::IsMouseButtonReleased(MouseButton button)
    {
        int index = static_cast<int>(button);
        return inputData.mouse.currentButtonState[index] == false && inputData.mouse.previousButtonState[index];
    }

    bool Input::IsMouseButtonDown(MouseButton button)
    {
        int index = static_cast<int>(button);
        return inputData.mouse.currentButtonState[index];
    }

    bool Input::IsMouseButtonUp(MouseButton button)
    {
        int index = static_cast<int>(button);
        return inputData.mouse.currentButtonState[index] == false;
    }

    void Input::SetMouseButtonState(MouseButton button, bool state)
    {
        DirectSetMouseButtonState(button, state);
    }

    const Vector2& Input::GetMousePosition()
    {
        return inputData.mouse.currentPosition;
    }

    const Vector2& Input::GetMouseDirection()
    {
        return inputData.mouse.direction;
    }

    void Input::SetMousePosition(const Vector2& position)
    {
        // Not calling direct function because it modifies the data differently

        glfwSetCursorPos(inputData.handler, position.x, position.y);
        inputData.mouse.currentPosition = position;
        MousePositionEvent event("MousePositionMove", position);
       inputData.eventManager.Emit(event);
    }

    // Scroll Virtual Device

    const Vector2& Input::GetScrollDirection()
    {
        return inputData.scroll.direction;
    }

    void Input::SetScrollDirection(const Vector2& direction)
    {
        DirectSetScrollDirection(direction);
    }

    void Input::Initialize()
    {
        inputData.handler = static_cast<GLFWwindow*>(Window::GetHandler());

		glfwSetKeyCallback(inputData.handler, [](GLFWwindow* handler, int vkeyCode, int scanCode, int action, int modidefrKey) 
		{
			Key key = static_cast<Key>(vkeyCode);
            Input::DirectSetKeyState(key, action == GLFW_PRESS || action == GLFW_REPEAT);
		});


		glfwSetMouseButtonCallback(inputData.handler, [](GLFWwindow* handler, int button, int action, int modidefrKey) 
		{
			MouseButton b = static_cast<MouseButton>(button);
            Input::DirectSetMouseButtonState(b, action == GLFW_PRESS);
		});

		glfwSetCursorPosCallback(inputData.handler, [](GLFWwindow* handler, double x, double y)
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
        return inputData.eventManager;
    }

    // Private Implementations

    void Input::UpdateKeyboardData()
    {
        for (int i = 0; i < inputData.keyboard.previousState.size(); ++i) 
        {
            inputData.keyboard.previousState[i] = inputData.keyboard.currentState[i];
        }
    }

    void Input::UpdateMouseData()
    {
        for (int i = 0; i < inputData.mouse.previousButtonState.size(); ++i) 
        {
            inputData.mouse.previousButtonState[i] = inputData.mouse.currentButtonState[i];
        }

        inputData.mouse.direction = inputData.mouse.currentPosition - inputData.mouse.previousPosition;
        inputData.mouse.previousPosition = inputData.mouse.currentPosition;
    }

    void Input::DirectSetKeyState(Key key, bool state)
    {
        int index = static_cast<int>(key);
        inputData.keyboard.currentState[index] = state;
        KeyEvent event(state ? "KeyPressed" : "KeyRelease", key);
       inputData.eventManager.Emit(event);
    }


    void Input::DirectSetMouseButtonState(MouseButton button, bool state)
    {
        inputData.mouse.currentButtonState[static_cast<int>(button)] = state;
        MouseButtonEvent event(state ? "MouseButtonPressed" : "MouseButtonRelease", button);
       inputData.eventManager.Emit(event);
    }

    void Input::DirectSetMousePosition(const Vector2& position)
    {
        inputData.mouse.currentPosition = position;
        MousePositionEvent event("MousePositionMove", position);
       inputData.eventManager.Emit(event);
    }

    void Input::DirectSetScrollDirection(const Vector2& direction)
    {
        inputData.scroll.direction = direction;
        ScrollEvent event("ScrollMove", direction);
       inputData.eventManager.Emit(event);
    }
}