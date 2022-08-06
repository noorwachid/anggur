#include "Input.h"
#include "Internal.h"
#include "Window.h"
#include <array>
#include <iostream>

namespace Anggur 
{
    // Input
        
    Input::Input(Window& newWindow): window(newWindow)
    {
        window.RegisterInput(this);
    }

    void Input::Update()
    {
    }

    // Keyboard

    Keyboard::Keyboard(Window& window): Input(window)
    {
		glfwSetKeyCallback(window.GetContext(), [](GLFWwindow* context, int vkeyCode, int scanCode, int action, int modifierKey) 
        {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			Key key = static_cast<Key>(vkeyCode);
            window->GetInput<Keyboard>().DirectSetKeyState(key, action == GLFW_PRESS || action == GLFW_REPEAT);
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
        window.emitter.Emit(event);
    }


    // Input mouse device

    Mouse::Mouse(Window& window): Input(window) 
    {
		glfwSetMouseButtonCallback(window.GetContext(), [](GLFWwindow* context, int button, int action, int modidefrKey) 
        {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			MouseButton b = static_cast<MouseButton>(button);
            window->GetInput<Mouse>().DirectSetButtonState(b, action == GLFW_PRESS);
		});

		glfwSetCursorPosCallback(window.GetContext(), [](GLFWwindow* context, double x, double y) 
        {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->GetInput<Mouse>().DirectSetPosition(Vector2(x, y));
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

    const Vector2& Mouse::GetPosition() 
    {
        return currentPosition;
    }

    const Vector2& Mouse::GetDirection() 
    {
        return direction;
    }

    MouseShape Mouse::GetShape()
    {
        return shape;
    }

    void Mouse::SetButtonState(MouseButton button, bool state) 
    {
        DirectSetButtonState(button, state);
    }

    void Mouse::SetShape(MouseShape newShape)
    {
        int intMode = static_cast<int>(newShape);
        if (shapeBuffers[intMode] == nullptr) {
            shapeBuffers[intMode] = glfwCreateStandardCursor(intMode + 0x00036000);
        }
        glfwSetCursor(window.GetContext(), static_cast<GLFWcursor*>(shapeBuffers[intMode]));
        shape = newShape;
    }

    void Mouse::SetPosition(const Vector2& position) 
    {
        // Not calling direct function because it modifies the data differently

        glfwSetCursorPos(window.GetContext(), position.x, position.y);
        currentPosition = position;
        MousePositionEvent event("MousePositionMove", position);
        window.emitter.Emit(event);
    }

    void Mouse::DirectSetButtonState(MouseButton button, bool state) {
        currentButtonState[static_cast<int>(button)] = state;
        MouseButtonEvent event(state ? "MouseButtonPressed" : "MouseButtonReleased", button);
        window.emitter.Emit(event);
    }

    void Mouse::DirectSetPosition(const Vector2& position) {
        currentPosition = position;
        MousePositionEvent event("MousePositionMoved", position);
        window.emitter.Emit(event);
    }

    // Scroller

    Scroller::Scroller(Window& window): Input(window)
    {
        glfwSetScrollCallback(window.GetContext(), [](GLFWwindow* context, double x, double y) 
        {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
            window->GetInput<Scroller>().DirectSetDirection(Vector2(x, y));
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
        window.emitter.Emit(event);
    }
}