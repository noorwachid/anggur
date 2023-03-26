#include "Anggur/IO/Input.h"
#include "Anggur/IO/Internal.h"
#include "Anggur/IO/Window.h"
#include <array>
#include <iostream>

namespace Anggur
{
	void Input::Initialize(WindowContext* newContext)
	{
		context = newContext;

		SetKeyCallbacks();
		SetMouseCallbacks();
	}

	void Input::SetKeyCallbacks()
	{
		// glfwSetKeyCallback(
		// 	context,
		// 	[](GLFWwindow* context, int vkeyCode, int scanCode, int state, int modifierKey)
		// 	{
		// 		Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
		// 		Key key = static_cast<Key>(vkeyCode);
		//
		// 		switch (state)
		// 		{
		// 			case GLFW_PRESS:
		// 			{
		// 				KeyPressedEvent event(key);
		// 				window.Dispatch(event);
		// 				break;
		// 			}
		//
		// 			case GLFW_REPEAT:
		// 			{
		// 				KeyHeldEvent event(key);
		// 				window.Dispatch(event);
		// 				break;
		// 			}
		//
		// 			case GLFW_RELEASE:
		// 			{
		// 				KeyReleasedEvent event(key);
		// 				window.Dispatch(event);
		// 				break;
		// 			}
		//
		// 			default:
		// 				break;
		// 		}
		// 	}
		// );
	}

	void Input::SetMouseCallbacks()
	{
		// glfwSetCursorPosCallback(
		// 	context,
		// 	[](GLFWwindow* context, double x, double y)
		// 	{
		// 		Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
		// 		MouseMovedEvent event(Vector2(x, y));
		// 		window.input.mousePosition = Vector2(x, y);
		// 		window.Dispatch(event);
		// 	}
		// );
		//
		// glfwSetMouseButtonCallback(
		// 	context,
		// 	[](GLFWwindow* context, int buttonCode, int state, int mods)
		// 	{
		// 		Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
		// 		MouseButton button = static_cast<MouseButton>(buttonCode);
		//
		// 		switch (state)
		// 		{
		// 			case GLFW_PRESS:
		// 			{
		// 				MousePressedEvent event(button);
		// 				window.Dispatch(event);
		// 				break;
		// 			}
		//
		// 			case GLFW_REPEAT:
		// 			{
		// 				MouseHeldEvent event(button);
		// 				window.Dispatch(event);
		// 				break;
		// 			}
		//
		// 			case GLFW_RELEASE:
		// 			{
		// 				MouseReleasedEvent event(button);
		// 				window.Dispatch(event);
		// 				break;
		// 			}
		//
		// 			default:
		// 				break;
		// 		}
		// 	}
		// );
	}

	bool Input::IsKeyPressed(Key key) const
	{
		int index = static_cast<int>(key);

		return glfwGetKey(context, index) == GLFW_PRESS;
	}

	bool Input::IsKeyHeld(Key key) const
	{
		int index = static_cast<int>(key);
		int state = glfwGetKey(context, index);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyReleased(Key key) const
	{
		int index = static_cast<int>(key);

		return glfwGetKey(context, index) == GLFW_RELEASE;
	}

	const Vector2& Input::GetMousePosition() const
	{
		return mousePosition;
	}
}
