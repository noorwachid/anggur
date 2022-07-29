#include "Input.h"
#include "Internal.h"
#include "Window.h"
#include <array>
#include <iostream>

namespace Anggur {
    // Keyboard device

    void Keyboard::update() {
        for (int i = 0; i < previousState.size(); ++i) 
        {
            previousState[i] = currentState[i];
        }
    }

    // Mouse device

    void Mouse::update() {
        for (int i = 0; i < previousButtonState.size(); ++i) 
        {
            previousButtonState[i] = currentButtonState[i];
        }

        direction = currentPosition - previousPosition;
        previousPosition = currentPosition;
    }


    // Scroll device

    void Scroll::update() {
        previousDirection = currentDirection;
        currentDirection.set(0.0f, 0.0f);
    }

    // Input keyboard device

    bool Input::isKeyPressed(Key key) {
        int index = static_cast<int>(key);
        return keyboard.currentState[index] && keyboard.previousState[index] == false;
    }

    bool Input::isKeyReleased(Key key) {
        int index = static_cast<int>(key);
        return keyboard.currentState[index] == false && keyboard.previousState[index];
    }

    bool Input::IsKeyDown(Key key) {
        int index = static_cast<int>(key);
        return keyboard.currentState[index];
    }

    bool Input::isKeyUp(Key key) {
        int index = static_cast<int>(key);
        return keyboard.currentState[index] == false;   
    }

    void Input::setKeyState(Key key, bool state) {
        directSetKeyState(key, state);
    }

    // Input mouse device

    bool Input::isMouseButtonPressed(MouseButton button) {
        int index = static_cast<int>(button);
        return mouse.currentButtonState[index] && mouse.previousButtonState[index] == false;
    }

    bool Input::isMouseButtonReleased(MouseButton button) {
        int index = static_cast<int>(button);
        return mouse.currentButtonState[index] == false && mouse.previousButtonState[index];
    }

    bool Input::isMouseButtonDown(MouseButton button) {
        int index = static_cast<int>(button);
        return mouse.currentButtonState[index];
    }

    bool Input::isMouseButtonUp(MouseButton button) {
        int index = static_cast<int>(button);
        return mouse.currentButtonState[index] == false;
    }

    void Input::setMouseButtonState(MouseButton button, bool state) {
        directSetMouseButtonState(button, state);
    }

    const Vector2& Input::getMousePosition() {
        return mouse.currentPosition;
    }

    const Vector2& Input::getMouseDirection() {
        return mouse.direction;
    }

    void Input::setMousePosition(const Vector2& position) {
        // Not calling direct function because it modifies the data differently

        glfwSetCursorPos(context, position.x, position.y);
        mouse.currentPosition = position;
        MousePositionEvent event("MousePositionMove", position);
        emitter.Emit(event);
    }

    // Input scroll virtual device

    const Vector2& Input::getScrollDirection() {
        return scroll.currentDirection;
    }

    void Input::setScrollDirection(const Vector2& direction) {
        directSetScrollDirection(direction);
    }

    // Context management

    void Input::setContext(WindowContext* newContext) {
        context = newContext;

		glfwSetKeyCallback(context, [](GLFWwindow* context, int vkeyCode, int scanCode, int action, int modidefrKey) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			Key key = static_cast<Key>(vkeyCode);
            window->input.directSetKeyState(key, action == GLFW_PRESS || action == GLFW_REPEAT);
		});


		glfwSetMouseButtonCallback(context, [](GLFWwindow* context, int button, int action, int modidefrKey) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			MouseButton b = static_cast<MouseButton>(button);
            window->input.directSetMouseButtonState(b, action == GLFW_PRESS);
		});

		glfwSetCursorPosCallback(context, [](GLFWwindow* context, double x, double y) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->input.directSetMousePosition(Vector2(x, y));
		});

        glfwSetScrollCallback(context, [](GLFWwindow* context, double x, double y) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
            window->input.directSetScrollDirection(Vector2(x, y));
        });
    }

    void Input::update() {
        keyboard.update();
        mouse.update();
        scroll.update();
    }

    // Private implementations

    void Input::directSetKeyState(Key key, bool state) {
        int index = static_cast<int>(key);
        keyboard.currentState[index] = state;
        KeyEvent event(state ? "keyPressed" : "keyReleased", key);
        emitter.Emit(event);
    }

    void Input::directSetMouseButtonState(MouseButton button, bool state) {
        mouse.currentButtonState[static_cast<int>(button)] = state;
        MouseButtonEvent event(state ? "mouseButtonPressed" : "wouseButtonReleased", button);
        emitter.Emit(event);
    }

    void Input::directSetMousePosition(const Vector2& position) {
        mouse.currentPosition = position;
        MousePositionEvent event("mousePositionMoved", position);
        emitter.Emit(event);
    }

    void Input::directSetScrollDirection(const Vector2& direction) {
        scroll.currentDirection = direction;
        ScrollEvent event("scrollMoved", direction);
        emitter.Emit(event);
    }
}