#include "Anggur/OS/Input.h"
#include "Anggur/OS/Internal.h"
#include "Anggur/OS/Window.h"
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

				if (window.observer == nullptr)
					return;

				Key key = static_cast<Key>(vkeyCode);

				switch (state)
				{
					case GLFW_PRESS:
					{
						KeyboardPressedEvent event;
						event.key = key;
						window.observer->OnKeyboardPress(event);
						break;
					}

					case GLFW_REPEAT:
					{
						KeyboardHeldEvent event;
						event.key = key;
						window.observer->OnKeyboardHold(event);
						break;
					}

					case GLFW_RELEASE:
					{
						KeyboardReleasedEvent event;
						event.key = key;
						window.observer->OnKeyboardRelease(event);
						break;
					}

					default:
						break;
				}
			}
		);

		glfwSetCharCallback(context, [](GLFWwindow* context, uint codepoint) {
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window.observer == nullptr)
				return;

			KeyboardTypedEvent event;
			event.codepoint = codepoint;
			window.observer->OnKeyboardType(event);
		});
	}

	void Input::SetMouseCallbacks()
	{
		glfwSetCursorPosCallback(
			context,
			[](GLFWwindow* context, double x, double y)
			{
				Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
				MouseMovedEvent event;
				event.position.Set(x, y);
				window.input.mousePosition.Set(x, y);
				window.observer->OnMouseMove(event);
			}
		);

		glfwSetMouseButtonCallback(
			context,
			[](GLFWwindow* context, int buttonCode, int state, int mods)
			{
				Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
				MouseButton button = static_cast<MouseButton>(buttonCode);

				switch (state)
				{
					case GLFW_PRESS:
					{
						MousePressedEvent event;
						event.button = button;
						window.observer->OnMousePress(event);
						break;
					}

					case GLFW_REPEAT:
					{
						MouseHeldEvent event;
						event.button = button;
						window.observer->OnMouseHold(event);
						break;
					}

					case GLFW_RELEASE:
					{
						MouseReleasedEvent event;
						event.button = button;
						window.observer->OnMouseRelease(event);
						break;
					}

					default:
						break;
				}
			}
		);
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
