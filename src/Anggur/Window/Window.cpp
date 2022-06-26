#include "Anggur/Graphics/Function.h"
#include "Anggur/Graphics/Image.h"
#include "Anggur/Utility/Assert.h"
#include "Anggur/Utility/Log.h"
#include "Window.h"
#include "Internal.h"
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

	WindowData windowData;

	void Window::Initialize(const Vector2& size, const std::string& title) 
	{
		int systemInitialization = glfwInit();
		ANGGUR_ASSERT(systemInitialization, "[Window] Failed to initalize window system");

		windowData.title = title;
		windowData.size = size;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef ANGGUR_OS_APPLE
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		windowData.handler = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
		ANGGUR_ASSERT(windowData.handler, "[Core.Window] Failed to create a window");

		Bind();
		InitializeGraphicsFunctions();

		int frameBufferWidth = 0;
		int frameBufferHeight = 0;
		glfwGetFramebufferSize(windowData.handler, &frameBufferWidth, &frameBufferHeight);
		windowData.frameBufferSize.set(frameBufferWidth, frameBufferHeight);

		glfwSetWindowSizeCallback(windowData.handler, [](GLFWwindow* handler, int width, int height)
		{
			Vector2 size(width, height);
			WindowSizeEvent event("WindowResize", size);
			windowData.eventManager.Emit(event);
			windowData.size = size;
		});

		glfwSetWindowPosCallback(windowData.handler, [](GLFWwindow* handler, int x, int y)
		{
			Vector2 position(x, y);
			WindowSizeEvent event("WindowMove", position);
			windowData.eventManager.Emit(event);
			windowData.position = position;
		});

		glfwSetFramebufferSizeCallback(windowData.handler, [](GLFWwindow* handler, int width, int height)
		{
			Vector2 size(width, height);
			FrameBufferSizeEvent event("FrameBufferResize", size);
			windowData.eventManager.Emit(event);
			windowData.frameBufferSize = size;
		});
	}

	void Window::SetPosition(const Vector2& position) 
	{
		glfwSetWindowPos(windowData.handler, position.x, position.y);
		windowData.position = position;
	}

	void Window::SetSize(const Vector2& size) 
	{
		glfwSetWindowSize(windowData.handler, size.x, size.y);
		windowData.size = size;
	}

	void Window::SetTitle(const std::string& title) 
	{
		glfwSetWindowTitle(windowData.handler, title.c_str());

		windowData.title = title;
	}

	void* Window::GetHandler()
	{
		return windowData.handler;
	}

	float Window::GetAspectRatio() 
	{
		return windowData.size.y / windowData.size.x;
	}

	const Vector2& Window::GetPosition() 
	{
		return windowData.position;
	}

	const Vector2& Window::GetSize() 
	{
		return windowData.size;
	}

	const Vector2& Window::GetFrameBufferSize() 
	{
		return windowData.frameBufferSize;
	}

	const std::string& Window::GetTitle() 
	{
		return windowData.title;
	}

	bool Window::IsOpen() 
	{
		return !glfwWindowShouldClose(windowData.handler);
	}

	void Window::SwapBuffers() 
	{
		glfwSwapBuffers(windowData.handler);
	}

	void Window::Close() 
	{
		glfwSetWindowShouldClose(windowData.handler, true);
	}

	void Window::Bind() 
	{
		glfwMakeContextCurrent(windowData.handler);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	EventManager& Window::GetEventManager()
	{
		return windowData.eventManager;
	}

	void Window::InitializeGraphicsFunctions() 
	{
		bool result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ANGGUR_ASSERT(result, "[Core.Window.load] Failed to load graphic functions");
	}
}
