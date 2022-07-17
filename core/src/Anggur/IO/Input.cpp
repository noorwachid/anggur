#include "Input.h"
#include "Internal.h"
#include "Window.h"
#include <array>
#include <iostream>

namespace Anggur 
{
    // Keyboard device

    void Keyboard::Update()
    {
        for (int i = 0; i < previousState.size(); ++i) 
        {
            previousState[i] = currentState[i];
        }
    }

    // Mouse device

    void Mouse::Update()
    {
        for (int i = 0; i < previousButtonState.size(); ++i) 
        {
            previousButtonState[i] = currentButtonState[i];
        }

        direction = currentPosition - previousPosition;
        previousPosition = currentPosition;
    }


    // Scroll device

    void Scroll::Update()
    {
        previousDirection = currentDirection;
        currentDirection.set(0.0f, 0.0f);
    }

    // Input keyboard device

    bool Input::IsKeyPressed(Key key)
    {
        int index = static_cast<int>(key);
        return keyboard.currentState[index] && keyboard.previousState[index] == false;
    }

    bool Input::IsKeyReleased(Key key)
    {
        int index = static_cast<int>(key);
        return keyboard.currentState[index] == false && keyboard.previousState[index];
    }

    bool Input::IsKeyDown(Key key)
    {
        int index = static_cast<int>(key);
        return keyboard.currentState[index];
    }

    bool Input::IsKeyUp(Key key)
    {
        int index = static_cast<int>(key);
        return keyboard.currentState[index] == false;   
    }

    void Input::SetKeyState(Key key, bool state)
    {
        DirectSetKeyState(key, state);
    }

    // Input mouse device

    bool Input::IsMouseButtonPressed(MouseButton button)
    {
        int index = static_cast<int>(button);
        return mouse.currentButtonState[index] && mouse.previousButtonState[index] == false;
    }

    bool Input::IsMouseButtonReleased(MouseButton button)
    {
        int index = static_cast<int>(button);
        return mouse.currentButtonState[index] == false && mouse.previousButtonState[index];
    }

    bool Input::IsMouseButtonDown(MouseButton button)
    {
        int index = static_cast<int>(button);
        return mouse.currentButtonState[index];
    }

    bool Input::IsMouseButtonUp(MouseButton button)
    {
        int index = static_cast<int>(button);
        return mouse.currentButtonState[index] == false;
    }

    void Input::SetMouseButtonState(MouseButton button, bool state)
    {
        DirectSetMouseButtonState(button, state);
    }

    const Vector2& Input::GetMousePosition()
    {
        return mouse.currentPosition;
    }

    const Vector2& Input::GetMouseDirection()
    {
        return mouse.direction;
    }

    void Input::SetMousePosition(const Vector2& position)
    {
        // Not calling direct function because it modifies the data differently

        glfwSetCursorPos(context, position.x, position.y);
        mouse.currentPosition = position;
        MousePositionEvent event("MousePositionMove", position);
        eventManager.Emit(event);
    }

    // Input scroll virtual device

    const Vector2& Input::GetScrollDirection()
    {
        return scroll.currentDirection;
    }

    void Input::SetScrollDirection(const Vector2& direction)
    {
        DirectSetScrollDirection(direction);
    }

    // Context management

    void Input::BindContext(WindowContext* newContext)
    {
        context = newContext;

		glfwSetKeyCallback(context, [](GLFWwindow* context, int vkeyCode, int scanCode, int action, int modidefrKey) 
		{
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			Key key = static_cast<Key>(vkeyCode);
            window->input.DirectSetKeyState(key, action == GLFW_PRESS || action == GLFW_REPEAT);
		});


		glfwSetMouseButtonCallback(context, [](GLFWwindow* context, int button, int action, int modidefrKey) 
		{
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			MouseButton b = static_cast<MouseButton>(button);
            window->input.DirectSetMouseButtonState(b, action == GLFW_PRESS);
		});

		glfwSetCursorPosCallback(context, [](GLFWwindow* context, double x, double y)
		{
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->input.DirectSetMousePosition(Vector2(x, y));
		});

        glfwSetScrollCallback(context, [](GLFWwindow* context, double x, double y) 
        {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
            window->input.DirectSetScrollDirection(Vector2(x, y));
        });
    }

    void Input::Update()
    {
        keyboard.Update();
        mouse.Update();
        scroll.Update();
    }

    // Private implementations

    void Input::DirectSetKeyState(Key key, bool state)
    {
        int index = static_cast<int>(key);
        keyboard.currentState[index] = state;
        KeyEvent event(state ? "KeyPressed" : "KeyRelease", key);
        eventManager.Emit(event);
    }

    void Input::DirectSetMouseButtonState(MouseButton button, bool state)
    {
        mouse.currentButtonState[static_cast<int>(button)] = state;
        MouseButtonEvent event(state ? "MouseButtonPressed" : "MouseButtonRelease", button);
        eventManager.Emit(event);
    }

    void Input::DirectSetMousePosition(const Vector2& position)
    {
        mouse.currentPosition = position;
        MousePositionEvent event("MousePositionMove", position);
        eventManager.Emit(event);
    }

    void Input::DirectSetScrollDirection(const Vector2& direction)
    {
        scroll.currentDirection = direction;
        ScrollEvent event("ScrollMove", direction);
        eventManager.Emit(event);
    }
}