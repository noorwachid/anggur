#include "Input.h"
#include "Internal.h"
#include "Window.h"
#include <array>
#include <iostream>

namespace Anggur
{
	// Input

	InputDevice::InputDevice(Window& newWindow) : window(newWindow)
	{
		window.RegisterInputDevice(this);
	}

	void InputDevice::Update()
	{
	}

	// Keyboard

	Keyboard::Keyboard(Window& window) : InputDevice(window)
	{
		glfwSetKeyCallback(window.GetContext(), [](GLFWwindow* context, int vkeyCode, int scanCode, int action,
												   int modifierKey) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			Key key = static_cast<Key>(vkeyCode);
			window->GetInputDevice<Keyboard>().DirectSetKeyState(key, action == GLFW_PRESS || action == GLFW_REPEAT);
		});
	}

	void Keyboard::Update()
	{
		for (int i = 0; i < previousState.size(); ++i)
			if (previousState[i] != currentState[i])
				previousState[i] = currentState[i];
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

	void Keyboard::DirectSetKeyState(Key key, bool state)
	{
		int index = static_cast<int>(key);
		currentState[index] = state;
		KeyEvent event(state ? "KeyPressed" : "KeyReleased", key);
		window.eventDispatcher.Dispatch(event);
	}

	// Input mouse device

	Mouse::Mouse(Window& window) : InputDevice(window)
	{
		glfwSetMouseButtonCallback(
			window.GetContext(), [](GLFWwindow* context, int button, int action, int modidefrKey) {
				auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
				MouseButton b = static_cast<MouseButton>(button);
				window->GetInputDevice<Mouse>().DirectSetButtonState(b, action == GLFW_PRESS);
			});

		glfwSetCursorPosCallback(window.GetContext(), [](GLFWwindow* context, double x, double y) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->GetInputDevice<Mouse>().DirectSetCursorPosition(Vector2(x, y));
		});

		glfwSetScrollCallback(window.GetContext(), [](GLFWwindow* context, double x, double y) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->GetInputDevice<Mouse>().DirectSetWheelDirection(Vector2(x, y));
		});
	}

	void Mouse::Update()
	{
		for (int i = 0; i < previousButtonState.size(); ++i)
			if (previousButtonState[i] != currentButtonState[i])
				previousButtonState[i] = currentButtonState[i];

		cursorDirection = currentCursorPosition - previousCursorPosition;
		previousCursorPosition = currentCursorPosition;

		previousWheelDirection = currentWheelDirection;
		currentWheelDirection.Set(0.0f, 0.0f);
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

	const Vector2& Mouse::GetCursorPosition()
	{
		return currentCursorPosition;
	}

	const Vector2& Mouse::GetCursorDirection()
	{
		return cursorDirection;
	}

	MouseCursorShape Mouse::GetCursorShape()
	{
		return shape;
	}

	const Vector2& Mouse::GetWheelDirection()
	{
		return currentWheelDirection;
	}

	void Mouse::SetButtonState(MouseButton button, bool state)
	{
		DirectSetButtonState(button, state);
	}

	void Mouse::SetCursorShape(MouseCursorShape newShape)
	{
		int intMode = static_cast<int>(newShape);
		if (shapeBuffers[intMode] == nullptr)
		{
			shapeBuffers[intMode] = glfwCreateStandardCursor(intMode + 0x00036000);
		}
		glfwSetCursor(window.GetContext(), static_cast<GLFWcursor*>(shapeBuffers[intMode]));
		shape = newShape;
	}

	void Mouse::SetCursorPosition(const Vector2& position)
	{
		// Not calling direct function because it modifies the data differently

		glfwSetCursorPos(window.GetContext(), position.x, position.y);
		currentCursorPosition = position;
		MousePositionEvent event("MousePositionMove", position);
		window.eventDispatcher.Dispatch(event);
	}

	void Mouse::SetWheelDirection(const Vector2& direction)
	{
		DirectSetWheelDirection(direction);
	}

	void Mouse::DirectSetButtonState(MouseButton button, bool state)
	{
		currentButtonState[static_cast<int>(button)] = state;
		MouseButtonEvent event(state ? "MouseButtonPressed" : "MouseButtonReleased", button);
		window.eventDispatcher.Dispatch(event);
	}

	void Mouse::DirectSetCursorPosition(const Vector2& position)
	{
		currentCursorPosition = position;
		MousePositionEvent event("MousePositionMoved", position);
		window.eventDispatcher.Dispatch(event);
	}

	void Mouse::DirectSetWheelDirection(const Vector2& direction)
	{
		currentWheelDirection = direction;
		ScrollEvent event("ScrollMoved", direction);
		window.eventDispatcher.Dispatch(event);
	}
}