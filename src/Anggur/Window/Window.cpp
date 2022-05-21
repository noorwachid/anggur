#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Image.h>
#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Window/Window.h>
#include <Anggur/Window/Internal.h>
#include <Anggur/Math/Vector2.h>

namespace Anggur 
{
	Window::Window(const Vector2& size, const std::string& title) 
	{
		this->title = title;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef ANGGUR_OS_APPLE
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		handler = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
		ANGGUR_ASSERT(handler, "[Core.Window] Failed to create a window");

		glfwSetWindowUserPointer(handler, static_cast<void*>(this));

		Bind();
		InitializeGraphicsFunctions();
		InitializeEventEmmiter();
	}

	Window::~Window() 
	{
		if (handler) {
			glfwDestroyWindow(handler);
		}
	}

	void Window::SetPosition(const Vector2& position) 
	{
		glfwSetWindowPos(handler, position.x, position.y);
	}

	void Window::SetSize(const Vector2& size) {
		glfwSetWindowSize(handler, size.x, size.y);
	}

	void Window::SetTitle(const std::string& title) {
		glfwSetWindowTitle(handler, title.c_str());

		this->title = title;
	}

	float Window::GetAspectRatio() {
		const Vector2& size = GetSize();

		return size.y / size.x;
	}

	const Vector2& Window::GetPosition() {
		int x, y;

		glfwGetWindowPos(handler, &x, &y);

		position.set(x, y);

		return position;
	}

	const Vector2& Window::GetSize() {
		int x, y;

		glfwGetWindowSize(handler, &x, &y);

		size.set(x, y);

		return size;
	}

	const std::string& Window::GetTitle() {
		return title;
	}

	bool Window::IsOpen() {
		return !glfwWindowShouldClose(handler);
	}

	void Window::SwapBuffers() {
		glfwSwapBuffers(handler);
	}

	WindowHandler* Window::GetHandler() {
		return handler;
	}

	void Window::Close() {
		glfwSetWindowShouldClose(handler, true);
	}

	void Window::Bind() {
		glfwMakeContextCurrent(handler);
	}

	void Window::InitializeGraphicsFunctions() {
		bool result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ANGGUR_ASSERT(result, "[Core.Window.load] Failed to load graphic functions");
	}

	void Window::InitializeEventEmmiter() 
	{
		glfwSetWindowSizeCallback(handler, [](WindowHandler* handler, int width, int height) 
		{
			WindowSizeEvent event(Vector2(width, height));
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handler));
			window->EmitEvent(event);
		});

		glfwSetFramebufferSizeCallback(handler, [](WindowHandler* handler, int width, int height) 
		{
			FrameBufferSizeEvent event(Vector2(width, height));
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handler));
			window->EmitEvent(event);
		});

		glfwSetCharCallback(handler, [](WindowHandler* handler, uint32_t codepoint) 
		{
			CodepointEvent event(codepoint);
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handler));
			window->EmitEvent(event);
		});
	}

	void Window::EmitEvent(Event& event) 
	{
		for (auto& eventListener: eventListeners) {
			eventListener(event);
		}
	}
}
