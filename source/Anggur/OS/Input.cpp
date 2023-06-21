#include "Anggur/OS/Input.h"
#include "Anggur/OS/Internal.h"
#include "Anggur/OS/Window.h"
#include "GLFW/glfw3.h"
#include <array>
#include <iostream>

namespace Anggur {
	void Input::initialize(WindowContext* newContext) {
		context = newContext;

		setKeyCallbacks();
		setMouseCallbacks();
	}

	void Input::setKeyCallbacks() {
		glfwSetKeyCallback(
			context,
			[](GLFWwindow* context, int vkeyCode, int scanCode, int state, int modifierKeyCode) {
				Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));

				if (window.observer == nullptr)
					return;

				KeyEvent event;
				event.modifierKey = static_cast<ModifierKey>(modifierKeyCode);
				event.key = static_cast<Key>(vkeyCode);

				switch (state) {
					case GLFW_PRESS:
						window.observer->onKeyPress(event);
						break;

					case GLFW_REPEAT:
						window.observer->onKeyHold(event);
						break;

					case GLFW_RELEASE:
						window.observer->onKeyRelease(event);
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

			TypeEvent event;
			event.codepoint = codepoint;
			window.observer->onType(event);
		});
	}

	void Input::setMouseCallbacks() {
		glfwSetCursorPosCallback(context, [](GLFWwindow* context, double x, double y) {
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
			MousePointerEvent event;
			event.position.set(x, y);
			window.input.mousePointer.set(x, y);
			window.observer->onMousePointerMove(event);
		});

		glfwSetMouseButtonCallback(context, [](GLFWwindow* context, int buttonCode, int state, int modifierKeyCode) {
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));
			MouseButtonEvent event;
			event.modifierKey = static_cast<ModifierKey>(modifierKeyCode);
			event.button = static_cast<MouseButton>(buttonCode);

			switch (state) {
				case GLFW_PRESS:
					window.observer->onMouseButtonPress(event);
					break;

				case GLFW_REPEAT:
					window.observer->onMouseButtonHold(event);
					break;

				case GLFW_RELEASE:
					window.observer->onMouseButtonRelease(event);
					break;

				default:
					break;
			}
		});

		glfwSetScrollCallback(context, [](GLFWwindow* context, double xOffset, double yOffset) {
			Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(context));

			ScrollEvent event;
			event.direction.set(xOffset, yOffset);
			window.observer->onScroll(event);
		});
	}

	bool Input::isKeyPressed(Key key) const {
		int index = static_cast<int>(key);

		return glfwGetKey(context, index) == GLFW_PRESS;
	}

	bool Input::isKeyHeld(Key key) const {
		int index = static_cast<int>(key);
		int state = glfwGetKey(context, index);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isKeyReleased(Key key) const {
		int index = static_cast<int>(key);

		return glfwGetKey(context, index) == GLFW_RELEASE;
	}

	const Vector2& Input::getMousePointer() const {
		return mousePointer;
	}
}
