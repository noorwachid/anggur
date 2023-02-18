#include "Input.h"
#include "Window.h"
#include "Internal.h"
#include <array>
#include <iostream>

namespace Anggur
{
	// Input
    
    void InputDevice::Initialize(WindowContext* newContext)
    {
        context = newContext;
    }

	void Keyboard::Initialize(WindowContext* newContext)
	{
        context = newContext;
		glfwSetKeyCallback(context, [](GLFWwindow* context, int vkeyCode, int scanCode, int state, int modifierKey) {
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
			Key key = static_cast<Key>(vkeyCode);

            switch (state) {
                case GLFW_PRESS:
                {
                    KeyEvent event("KeyPressed", key);
                    window.Dispatch(event);
                    break;
                }

                case GLFW_REPEAT:
                {
                    KeyEvent event("KeyDown", key);
                    window.Dispatch(event);
                    break;
                }

                case GLFW_RELEASE:
                {
                    KeyEvent event("KeyReleased", key);
                    window.Dispatch(event);
                    break;
                }

                default: break;
            }
		});
	}

	bool Keyboard::IsKeyPressed(Key key) const
	{
		int index = static_cast<int>(key);

        return glfwGetKey(context, index) == GLFW_PRESS;
	}

	bool Keyboard::IsKeyReleased(Key key) const
	{
		int index = static_cast<int>(key);

        return glfwGetKey(context, index) == GLFW_RELEASE;
	}

	bool Keyboard::IsKeyDown(Key key) const
	{
		int index = static_cast<int>(key);
        int state = glfwGetKey(context, index);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Keyboard::IsKeyUp(Key key) const
	{
		int index = static_cast<int>(key);

        return IsKeyReleased(key) || !IsKeyDown(key);
	}

	void Mouse::Initialize(WindowContext* newContext)
	{
        context = newContext;
		glfwSetCursorPosCallback(context, [](GLFWwindow* context, double x, double y) {
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
            MousePositionEvent event("MouseMoved", Vector2(x, y));
            window.Dispatch(event);
		});
	}
}
