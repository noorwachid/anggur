#include "Anggur/OS/Input.h"
#include "Anggur/OS/Internal.h"
#include "Anggur/OS/Window.h"
#include "GLFW/glfw3.h"
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
			[](GLFWwindow* context, int vkeyCode, int scanCode, int state, int modifierKeyCode)
			{
				Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));

				if (window.observer == nullptr)
					return;

				KeyEvent event; 
				event.modifierKey = static_cast<ModifierKey>(modifierKeyCode);
				event.key = static_cast<Key>(vkeyCode);

				switch (state)
				{
					case GLFW_PRESS:
						window.observer->OnKeyPress(event);
						break;

					case GLFW_REPEAT:
						window.observer->OnKeyHold(event);
						break;

					case GLFW_RELEASE:
						window.observer->OnKeyRelease(event);
						break;

					default:
						break;
				}
			}
		);

		glfwSetCharCallback(context, [](GLFWwindow* context, uint codepoint) {
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window.observer == nullptr)
				return;

			TextEvent event;
			event.codepoint = codepoint;
			window.observer->OnText(event);
		});
	}

	void Input::SetMouseCallbacks()
	{
		glfwSetCursorPosCallback(
			context,
			[](GLFWwindow* context, double x, double y)
			{
				Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
				MousePointerEvent event;
				event.position.Set(x, y);
				window.input.mousePointer.Set(x, y);
				window.observer->OnMousePointerMove(event);
			}
		);

		glfwSetMouseButtonCallback(
			context,
			[](GLFWwindow* context, int buttonCode, int state, int mods)
			{
				Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
				MouseButtonEvent event;
				event.button = static_cast<MouseButton>(buttonCode);

				switch (state)
				{
					case GLFW_PRESS:
						window.observer->OnMouseButtonPress(event);
						break;

					case GLFW_REPEAT:
						window.observer->OnMouseButtonHold(event);
						break;

					case GLFW_RELEASE:
						window.observer->OnMouseButtonRelease(event);
						break;

					default:
						break;
				}
			}
		);

		glfwSetScrollCallback(
			context, 
			[](GLFWwindow* context, double xOffset, double yOffset)
			{
				Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
				
				ScrollEvent event;
				event.direction.Set(xOffset, yOffset);
				window.observer->OnScroll(event);
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

	const Vector2& Input::GetMousePointer() const
	{
		return mousePointer;
	}
}
