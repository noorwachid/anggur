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
		glfwSetKeyCallback(
			context,
			[](GLFWwindow* context, int vkeyCode, int scanCode, int state, int modifierKey)
			{
				Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));

				if (window.listener == nullptr)
					return;

				Key key = static_cast<Key>(vkeyCode);

				switch (state)
				{
					case GLFW_PRESS:
					{
						KeyboardPressedEvent event;
						event.key = key;
						window.listener->OnKeyboardPress(event);
						break;
					}

					case GLFW_REPEAT:
					{
						KeyboardHeldEvent event;
						event.key = key;
						window.listener->OnKeyboardHold(event);
						break;
					}

					case GLFW_RELEASE:
					{
						KeyboardReleasedEvent event;
						event.key = key;
						window.listener->OnKeyboardRelease(event);
						break;
					}

					default:
						break;
				}
			}
		);

		glfwSetCharCallback(context, [](GLFWwindow* context, uint codepoint) {
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window.listener == nullptr)
				return;

			KeyboardTypedEvent event;
			event.codepoint = codepoint;
			window.listener->OnKeyboardType(event);
		});
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

	bool Input::IsKeyboardPressed(Key key) const
	{
		int index = static_cast<int>(key);

		return glfwGetKey(context, index) == GLFW_PRESS;
	}

	bool Input::IsKeyboardHeld(Key key) const
	{
		int index = static_cast<int>(key);
		int state = glfwGetKey(context, index);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyboardReleased(Key key) const
	{
		int index = static_cast<int>(key);

		return glfwGetKey(context, index) == GLFW_RELEASE;
	}

	const Vector2& Input::GetMousePosition() const
	{
		return mousePosition;
	}
}
