#include "Anggur/Graphics/Function.h"
#include "Anggur/IO/Internal.h"
#include "Anggur/IO/Window.h"
#include <cassert>

namespace Anggur
{
	Window::Window(const Vector2& newSize, const std::string& newTitle) : size(newSize), title(newTitle)
	{
#ifdef ANGGUR_OS_X
		glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, 0);
#endif

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef ANGGUR_OS_X
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		context = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
		assert(context && "[Window] Failed to create a window");

		BindContext();
		BindGraphicsAPI();

		InitializeGraphicsAPI();

		glEnable(GL_MULTISAMPLE);

		glfwSetWindowSizeCallback(
			context,
			[](GLFWwindow* context, int width, int height)
			{
				auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
				Vector2 size(width, height);
				WindowResizedEvent event(size);
				window->Dispatch(event);
				window->size = size;
			}
		);

		glfwSetFramebufferSizeCallback(
			context,
			[](GLFWwindow* context, int width, int height)
			{
				auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
				Vector2 size(width, height);
				WindowResizedEvent event(size);
				window->Dispatch(event);
				window->frameBufferSize = size;
			}
		);

		glfwSetWindowPosCallback(
			context,
			[](GLFWwindow* context, int x, int y)
			{
				auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
				Vector2 position(x, y);
				WindowMovedEvent event(position);
				window->Dispatch(event);
				window->position = position;
			}
		);

		int fbx, fby;
		glfwGetFramebufferSize(context, &fbx, &fby);
		frameBufferSize.Set(fbx, fby);

		input.Initialize(context);
	}

	Window::~Window()
	{
		if (context)
			glfwDestroyWindow(context);
	}

	WindowContext* Window::GetContext()
	{
		return context;
	}

	float Window::GetAspectRatio()
	{
		return size.y / size.x;
	}

	float Window::GetScale()
	{
		return frameBufferSize.x / size.x;
	}

	const Vector2& Window::GetPosition()
	{
		return position;
	}

	const Vector2& Window::GetSize()
	{
		return size;
	}

	const Vector2& Window::GetFrameBufferSize()
	{
		return frameBufferSize;
	}

	const std::string& Window::GetTitle()
	{
		return title;
	}

	void Window::SetPosition(const Vector2& position)
	{
		glfwSetWindowPos(context, position.x, position.y);
	}

	void Window::SetSize(const Vector2& size)
	{
		glfwSetWindowSize(context, size.x, size.y);
	}

	void Window::SetTitle(const std::string& newTitle)
	{
		glfwSetWindowTitle(context, newTitle.c_str());
		title = newTitle;
	}

	bool Window::IsOpen()
	{
		return !glfwWindowShouldClose(context);
	}

	void Window::Close()
	{
		glfwSetWindowShouldClose(context, true);
	}

	void Window::Update()
	{
		BindGraphicsAPI();
		SwapFrameBuffers();
	}

	void Window::InitializeGraphicsAPI()
	{
		bool result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		assert(result && "[Window] Failed to load graphic functions");
	}

	void Window::BindGraphicsAPI()
	{
		glfwMakeContextCurrent(context);
	}

	void Window::SwapFrameBuffers()
	{
		glfwSwapBuffers(context);
	}

	void Window::BindContext()
	{
		glfwSetWindowUserPointer(context, this);
	}
}
