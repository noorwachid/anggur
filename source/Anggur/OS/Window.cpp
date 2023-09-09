#include "GLFW/glfw3.h"
#include "Anggur/Log.h"
#include "Anggur/OS/Window.h"

namespace Anggur
{
	Window::Window(uint32_t width, uint32_t height, const std::string& title) : _width(width), _height(height), _title(title)
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		_context = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);

		if (_context == NULL)
		{
			ANGGUR_LOG("failed to spawn new window")

			glfwTerminate();
			std::exit(1);
			return;
		}

		glfwMakeContextCurrent(_context);
	}

	Window::~Window()
	{
		glfwDestroyWindow(_context);
		glfwTerminate();
	}

	uint32_t Window::GetWidth() const
	{
		return _width;
	}

	uint32_t Window::GetHeight() const
	{
		return _height;
	}

	const std::string Window::GetTitle() const
	{
		return _title;
	}

	WindowContext* Window::GetContext()
	{
		return _context;
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
		glfwSetWindowUserPointer(_context, observer);

		glfwSetWindowSizeCallback(_context, [](GLFWwindow* context, int width, int height) 
		{
			WindowObserver* observer = static_cast<WindowObserver*>(glfwGetWindowUserPointer(context));

			if (observer)
			{
				WindowSize size;
				size.width = width;
				size.height = height;

				observer->OnWindowResize(size);
			}
		});

		glfwSetFramebufferSizeCallback(_context, [](GLFWwindow* context, int width, int height) 
		{
			WindowObserver* observer = static_cast<WindowObserver*>(glfwGetWindowUserPointer(context));
			// NOTE: capture the new value
		});

		glfwSetWindowPosCallback(_context, [](GLFWwindow* context, int x, int y) 
		{
			WindowObserver* observer = static_cast<WindowObserver*>(glfwGetWindowUserPointer(context));

			if (observer)
			{
				WindowPosition position;
				position.x = x;
				position.y = y;

				observer->OnWindowMove(position);
			}
		});


		glfwSetKeyCallback(_context, [](WindowContext* context, int key, int scancode, int action, int modKey) {
			WindowObserver* observer = static_cast<WindowObserver*>(glfwGetWindowUserPointer(context));

			if (observer)
			{
				if (action == GLFW_PRESS)
					observer->OnKeyPress(static_cast<Key>(key));

				if (action == GLFW_REPEAT)
					observer->OnKeyHold(static_cast<Key>(key));

				if (action == GLFW_RELEASE)
					observer->OnKeyRelease(static_cast<Key>(key));
			}
		});

		glfwSetMouseButtonCallback(_context, [](WindowContext* context, int button, int action, int modKey) {
			WindowObserver* observer = static_cast<WindowObserver*>(glfwGetWindowUserPointer(context));

			if (observer)
			{
				if (action == GLFW_PRESS)
					observer->OnMousePress(static_cast<MouseButton>(button));

				if (action == GLFW_REPEAT)
					observer->OnMouseHold(static_cast<MouseButton>(button));

				if (action == GLFW_RELEASE)
					observer->OnMouseRelease(static_cast<MouseButton>(button));
			}
		});

		glfwSetCursorPosCallback(_context, [](WindowContext* context, double x, double y) {
			WindowObserver* observer = static_cast<WindowObserver*>(glfwGetWindowUserPointer(context));

			if (observer)
			{
				MousePosition position;
				position.x = x;
				position.y = y;

				observer->OnMouseMove(position);
			}
		});

		glfwSetScrollCallback(_context, [](WindowContext* context, double x, double y) {
			WindowObserver* observer = static_cast<WindowObserver*>(glfwGetWindowUserPointer(context));

			if (observer)
			{
				MouseScroll scroll;
				scroll.x = x;
				scroll.y = y;

				observer->OnMouseScroll(scroll);
			}
		});
	}
}
