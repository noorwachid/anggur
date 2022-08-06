#include "Input.h"
#include "Internal.h"
#include "Window.h"
#include <array>
#include <iostream>

namespace Anggur 
{
    // Input
        
    Input::Input(InputSystem* systemRef): system(systemRef)
    {
        system->references.push_back(this);
    }

    void Input::Update()
    {
    }

    // Input System

    void InputSystem::SetContext(WindowContext* newContext) 
    {
        context = newContext;

        // glfwSetCharCallback(context, [](GLFWwindow* context, unsigned int codePoint) 
        // {
        //     auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
        //     CodePointEvent event(codePoint);
        //     window->input.emitter.Emit(event);
        // });
    }

    void InputSystem::Update() {
        for (Input* input: references) {
            input->Update();
        }
    }

    // Keyboard

    Keyboard::Keyboard(InputSystem* system): Input(system)
    {
		glfwSetKeyCallback(system->context, [](GLFWwindow* context, int vkeyCode, int scanCode, int action, int modidefrKey) 
        {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			Key key = static_cast<Key>(vkeyCode);
            window->input.Get<Keyboard>().DirectSetKeyState(key, action == GLFW_PRESS || action == GLFW_REPEAT);
		});
    }

    void Keyboard::Update() 
    {
        for (int i = 0; i < previousState.size(); ++i) 
        {
            previousState[i] = currentState[i];
        }
    }

    bool Keyboard::IsKeyPressed(Key key) 
    {
        int index = static_cast<int>(key);
        return currentState[index] && previousState[index] == false;
    }

    bool Keyboard::IsKeyReleased(Key key) 
    {
        int index = static_cast<int>(key);
        return currentState[index] == false && previousState[index];
    }

    bool Keyboard::IsKeyDown(Key key) 
    {
        int index = static_cast<int>(key);
        return currentState[index];
    }

    bool Keyboard::IsKeyUp(Key key) 
    {
        int index = static_cast<int>(key);
        return currentState[index] == false;   
    }

    void Keyboard::SetKeyState(Key key, bool state) 
    {
        DirectSetKeyState(key, state);
    }

    void Keyboard::DirectSetKeyState(Key key, bool state) {
        int index = static_cast<int>(key);
        currentState[index] = state;
        KeyEvent event(state ? "KeyPressed" : "KeyReleased", key);
        system->emitter.Emit(event);
    }


    // Input mouse device

    Mouse::Mouse(InputSystem* system): Input(system) 
    {
		glfwSetMouseButtonCallback(system->context, [](GLFWwindow* context, int button, int action, int modidefrKey) 
        {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			MouseButton b = static_cast<MouseButton>(button);
            window->input.Get<Mouse>().DirectSetButtonState(b, action == GLFW_PRESS);
		});

		glfwSetCursorPosCallback(system->context, [](GLFWwindow* context, double x, double y) 
        {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->input.Get<Mouse>().DirectSetPosition(Vector2(x, y));
		});
    }

    void Mouse::Update() 
    {
        for (int i = 0; i < previousButtonState.size(); ++i) 
        {
            previousButtonState[i] = currentButtonState[i];
        }

        direction = currentPosition - previousPosition;
        previousPosition = currentPosition;
    }

    bool Mouse::IsButtonPressed(MouseButton button) 
    {
        int index = static_cast<int>(button);
        return currentButtonState[index] && previousButtonState[index] == false;
    }

    bool Mouse::IsButtonReleased(MouseButton button) 
    {
        int index = static_cast<int>(button);
        return currentButtonState[index] == false && previousButtonState[index];
    }

    bool Mouse::IsButtonDown(MouseButton button) 
    {
        int index = static_cast<int>(button);
        return currentButtonState[index];
    }

    bool Mouse::IsButtonUp(MouseButton button) 
    {
        int index = static_cast<int>(button);
        return currentButtonState[index] == false;
    }

    void Mouse::SetButtonState(MouseButton button, bool state) 
    {
        DirectSetButtonState(button, state);
    }

    const Vector2& Mouse::GetPosition() 
    {
        return currentPosition;
    }

    const Vector2& Mouse::GetDirection() 
    {
        return direction;
    }

    void Mouse::SetPosition(const Vector2& position) 
    {
        // Not calling direct function because it modifies the data differently

        glfwSetCursorPos(system->context, position.x, position.y);
        currentPosition = position;
        MousePositionEvent event("MousePositionMove", position);
        system->emitter.Emit(event);
    }

    void Mouse::DirectSetButtonState(MouseButton button, bool state) {
        currentButtonState[static_cast<int>(button)] = state;
        MouseButtonEvent event(state ? "MouseButtonPressed" : "MouseButtonReleased", button);
        system->emitter.Emit(event);
    }

    void Mouse::DirectSetPosition(const Vector2& position) {
        currentPosition = position;
        MousePositionEvent event("MousePositionMoved", position);
        system->emitter.Emit(event);
    }

    // Scroller

    Scroller::Scroller(InputSystem* system): Input(system)
    {
        glfwSetScrollCallback(system->context, [](GLFWwindow* context, double x, double y) 
        {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
            window->input.Get<Scroller>().DirectSetDirection(Vector2(x, y));
        });
    }

    void Scroller::Update() 
    {
        previousDirection = currentDirection;
        currentDirection.Set(0.0f, 0.0f);
    }

    const Vector2& Scroller::GetDirection() 
    {
        return currentDirection;
    }

    void Scroller::SetDirection(const Vector2& direction) 
    {
        DirectSetDirection(direction);
    }
    
    void Scroller::DirectSetDirection(const Vector2& direction) {
        currentDirection = direction;
        ScrollEvent event("ScrollMoved", direction);
        system->emitter.Emit(event);
    }
}