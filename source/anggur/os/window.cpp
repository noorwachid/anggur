#include "anggur/os/window.h"
#include "anggur/log.h"
#include "GLFW/glfw3.h"

namespace Anggur {
	Window::Window(uint32_t width, uint32_t height, const std::string& title)
		: width(width), height(height), title(title) {
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		context = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (context == NULL) {
			ANGGUR_LOG("failed to spawn new window")

			glfwTerminate();
			std::exit(1);
			return;
		}

		glfwMakeContextCurrent(context);
		glfwSetWindowUserPointer(context, this);

		int fbWidth;
		int fbHeight;
		glfwGetFramebufferSize(context, &fbWidth, &fbHeight);
		frameBufferWidth = fbWidth;
		frameBufferHeight = fbHeight;
	}

	Window::~Window() {
		glfwDestroyWindow(context);
		glfwTerminate();
	}

	unsigned int Window::getWidth() const {
		return width;
	}

	unsigned int Window::getHeight() const {
		return height;
	}

	unsigned int Window::getFrameBufferWidth() const {
		return frameBufferWidth;
	}

	unsigned int Window::getFrameBufferHeight() const {
		return frameBufferHeight;
	}

	const std::string Window::getTitle() const {
		return title;
	}

	WindowContext* Window::getContext() {
		return context;
	}

	Input* Window::getInput() {
		return &input;
	}

	bool Window::shouldClose() const {
		return glfwWindowShouldClose(context);
	}

	void Window::swapBuffers() {
		return glfwSwapBuffers(context);
	}

	void Window::pollEvents() {
		glfwPollEvents();
	}

	void Window::connect(std::function<bool(void*)> connector) {
		bool result = connector(reinterpret_cast<void*>(glfwGetProcAddress));

		if (!result) {
			ANGGUR_LOG("failed to connect the graphics API")
		}
	}

	void Window::close() {
		glfwSetWindowShouldClose(context, true);
	}

	void Window::setObserver(WindowObserver* observer) {
		this->observer = observer;

		glfwSetWindowSizeCallback(context, [](GLFWwindow* context, int width, int height) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->observer) {
				WindowSize size;
				size.width = width;
				size.height = height;

				window->width = width;
				window->height = height;
				window->observer->onWindowResize(size);
			}
		});

		glfwSetFramebufferSizeCallback(context, [](GLFWwindow* context, int width, int height) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->frameBufferWidth = width;
			window->frameBufferHeight = height;
		});

		glfwSetWindowPosCallback(context, [](GLFWwindow* context, int x, int y) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->observer) {
				WindowPosition position;
				position.x = x;
				position.y = y;

				window->observer->onWindowMove(position);
			}
		});

		glfwSetKeyCallback(context, [](WindowContext* context, int key, int scancode, int action, int modKey) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->observer) {
				if (action == GLFW_PRESS) {
					window->observer->onKeyPress(static_cast<Key>(key));
					window->input.modifierKey = static_cast<ModifierKey>(modKey);
				}

				if (action == GLFW_REPEAT) {
					window->observer->onKeyHold(static_cast<Key>(key));
				}

				if (action == GLFW_RELEASE) {
					window->observer->onKeyRelease(static_cast<Key>(key));
					window->input.modifierKey = static_cast<ModifierKey>(modKey);
				}
			}
		});

		glfwSetCharCallback(context, [](WindowContext* context, unsigned int codepoint) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->observer) {
				window->observer->onType(codepoint);
			}
		});

		glfwSetMouseButtonCallback(context, [](WindowContext* context, int button, int action, int modKey) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->observer) {
				if (action == GLFW_PRESS)
					window->observer->onMousePress(static_cast<MouseButton>(button));

				if (action == GLFW_REPEAT)
					window->observer->onMouseHold(static_cast<MouseButton>(button));

				if (action == GLFW_RELEASE)
					window->observer->onMouseRelease(static_cast<MouseButton>(button));
			}
		});

		glfwSetCursorPosCallback(context, [](WindowContext* context, double x, double y) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->observer) {
				MousePosition position;
				position.x = x;
				position.y = y;

				window->observer->onMouseMove(position);
			}
		});

		glfwSetScrollCallback(context, [](WindowContext* context, double x, double y) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->observer) {
				MouseScroll scroll;
				scroll.x = x;
				scroll.y = y;

				window->observer->onMouseScroll(scroll);
			}
		});
	}
}
