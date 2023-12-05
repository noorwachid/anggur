#include "Anggur/OS/Window.h"
#include "GLFW/glfw3.h"
#include "Anggur/Log.h"

namespace Anggur
{
	Window::Window(unsigned int width, unsigned int height, const std::string& title)
		: _width(width), _height(height), _title(title)
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		_context = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (_context == NULL)
		{
			ANGGUR_LOG("failed to spawn new window")

			glfwTerminate();
			std::exit(1);
			return;
		}

		glfwMakeContextCurrent(_context);
		glfwSetWindowUserPointer(_context, this);

		int fbWidth;
		int fbHeight;
		glfwGetFramebufferSize(_context, &fbWidth, &fbHeight);
		_frameBufferWidth = fbWidth;
		_frameBufferHeight = fbHeight;
	}

	Window::~Window()
	{
		glfwDestroyWindow(_context);
		glfwTerminate();
	}

	unsigned int Window::GetWidth() const
	{
		return _width;
	}

	unsigned int Window::GetHeight() const
	{
		return _height;
	}

	unsigned int Window::GetFrameBufferWidth() const
	{
		return _frameBufferWidth;
	}

	unsigned int Window::GetFrameBufferHeight() const
	{
		return _frameBufferHeight;
	}

	const std::string Window::GetTitle() const
	{
		return _title;
	}

	WindowContext* Window::GetContext()
	{
		return _context;
	}

	Input* Window::GetInput()
	{
		return &_input;
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(_context);
	}

	void Window::SwapBuffers()
	{
		return glfwSwapBuffers(_context);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::Connect(std::function<bool(void*)> connector)
	{
		bool result = connector(reinterpret_cast<void*>(glfwGetProcAddress));

		if (!result)
		{
			ANGGUR_LOG("failed to connect the graphics API")
		}
	}

	void Window::Close()
	{
		glfwSetWindowShouldClose(_context, true);
	}

	void Window::SetObserver(WindowObserver* observer)
	{
		this->_observer = observer;

		glfwSetWindowSizeCallback(_context, [](GLFWwindow* context, int width, int height) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->_observer)
			{
				WindowSize size;
				size.width = width;
				size.height = height;

				window->_width = width;
				window->_height = height;
				window->_observer->OnWindowResize(size);
			}
		});

		glfwSetFramebufferSizeCallback(_context, [](GLFWwindow* context, int width, int height) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->_frameBufferWidth = width;
			window->_frameBufferHeight = height;
		});

		glfwSetWindowPosCallback(_context, [](GLFWwindow* context, int x, int y) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->_observer)
			{
				WindowPosition position;
				position.x = x;
				position.y = y;

				window->_observer->OnWindowMove(position);
			}
		});

		glfwSetKeyCallback(_context, [](WindowContext* context, int key, int scancode, int action, int modKey) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->_observer)
			{
				if (action == GLFW_PRESS)
				{
					window->_observer->OnKeyPress(static_cast<Key>(key));
					window->_input._modifierKey = static_cast<ModifierKey>(modKey);
				}

				if (action == GLFW_REPEAT)
				{
					window->_observer->OnKeyHold(static_cast<Key>(key));
				}

				if (action == GLFW_RELEASE)
				{
					window->_observer->OnKeyRelease(static_cast<Key>(key));
					window->_input._modifierKey = static_cast<ModifierKey>(modKey);
				}
			}
		});

		glfwSetCharCallback(_context, [](WindowContext* context, unsigned int codepoint) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->_observer)
			{
				window->_observer->OnType(codepoint);
			}
		});

		glfwSetMouseButtonCallback(_context, [](WindowContext* context, int button, int action, int modKey) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->_observer)
			{
				if (action == GLFW_PRESS)
					window->_observer->OnMousePress(static_cast<MouseButton>(button));

				if (action == GLFW_REPEAT)
					window->_observer->OnMouseHold(static_cast<MouseButton>(button));

				if (action == GLFW_RELEASE)
					window->_observer->OnMouseRelease(static_cast<MouseButton>(button));
			}
		});

		glfwSetCursorPosCallback(_context, [](WindowContext* context, double x, double y) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->_observer)
			{
				MousePosition position;
				position.x = x;
				position.y = y;

				window->_observer->OnMouseMove(position);
			}
		});

		glfwSetScrollCallback(_context, [](WindowContext* context, double x, double y) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));

			if (window->_observer)
			{
				MouseScroll scroll;
				scroll.x = x;
				scroll.y = y;

				window->_observer->OnMouseScroll(scroll);
			}
		});
	}
}
