#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Image.h>
#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Event/EventManager.h>
#include <Anggur/Window/Window.h>
#include <Anggur/Window/Internal.h>
#include <Anggur/Window/WindowEvent.h>
#include <Anggur/Math/Vector2.h>
#include <iostream>

namespace Anggur 
{
	struct WindowData 
	{
		GLFWwindow* handler = nullptr;
		std::string title;

		Vector2 position;
		Vector2 size;
		Vector2 frameBufferSize;
		EventManager eventManager;
	};

	WindowData data;

	void Window::Initialize(const Vector2& size, const std::string& title) 
	{
		int systemInitialization = glfwInit();
		ANGGUR_ASSERT(systemInitialization, "[Window] Failed to initalize window system");

		data.title = title;
		data.size = size;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef ANGGUR_OS_APPLE
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		data.handler = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
		ANGGUR_ASSERT(data.handler, "[Core.Window] Failed to create a window");

		Bind();
		InitializeGraphicsFunctions();

		glfwSetWindowSizeCallback(data.handler, [](GLFWwindow* handler, int width, int height)
		{
			Vector2 size(width, height);
			WindowSizeEvent event("WindowResize", size);
			data.eventManager.Emit(event);
			data.size = size;
		});

		glfwSetWindowPosCallback(data.handler, [](GLFWwindow* handler, int x, int y)
		{
			Vector2 position(x, y);
			WindowSizeEvent event("WindowMove", position);
			data.eventManager.Emit(event);
			data.position = position;
		});

		glfwSetFramebufferSizeCallback(data.handler, [](GLFWwindow* handler, int width, int height)
		{
			Vector2 size(width, height);
			FrameBufferSizeEvent event("FrameBufferResize", size);
			data.eventManager.Emit(event);
			data.frameBufferSize = size;
		});
	}

	void Window::SetPosition(const Vector2& position) 
	{
		glfwSetWindowPos(data.handler, position.x, position.y);
		data.position = position;
	}

	void Window::SetSize(const Vector2& size) 
	{
		glfwSetWindowSize(data.handler, size.x, size.y);
		data.size = size;
	}

	void Window::SetTitle(const std::string& title) 
	{
		glfwSetWindowTitle(data.handler, title.c_str());

		data.title = title;
	}

	void* Window::GetHandler()
	{
		return data.handler;
	}

	float Window::GetAspectRatio() 
	{
		return data.size.y / data.size.x;
	}

	const Vector2& Window::GetPosition() 
	{
		return data.position;
	}

	const Vector2& Window::GetSize() 
	{
		return data.size;
	}

	const Vector2& Window::GetFrameBufferSize() 
	{
		return data.frameBufferSize;
	}

	const std::string& Window::GetTitle() 
	{
		return data.title;
	}

	bool Window::IsOpen() 
	{
		return !glfwWindowShouldClose(data.handler);
	}

	void Window::SwapBuffers() 
	{
		glfwSwapBuffers(data.handler);
	}

	void Window::Close() 
	{
		glfwSetWindowShouldClose(data.handler, true);
	}

	void Window::Bind() 
	{
		glfwMakeContextCurrent(data.handler);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	EventManager& Window::GetEventManager()
	{
		return data.eventManager;
	}

	void Window::InitializeGraphicsFunctions() 
	{
		bool result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ANGGUR_ASSERT(result, "[Core.Window.load] Failed to load graphic functions");
	}
}
