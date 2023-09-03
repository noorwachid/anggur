#include "GLFW/glfw3.h"
#include "Anggur/Log.h"
#include "Anggur/OS/Window.h"

namespace Anggur
{
	Window::Window(uint32_t width, uint32_t height) : _width(width), _height(height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		_context = glfwCreateWindow(_width, _height, "", NULL, NULL);

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

	void Window::Connect(std::function<bool(void*)> client)
	{
		bool result = client(reinterpret_cast<void*>(glfwGetProcAddress));

		if (!result)
		{
			ANGGUR_LOG("failed to connect the graphics API")
		}
	}

	void Window::Close()
	{
		glfwSetWindowShouldClose(_context, true);
	}
}
