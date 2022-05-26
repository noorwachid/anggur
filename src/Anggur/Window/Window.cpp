#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Image.h>
#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Window/Window.h>
#include <Anggur/Window/Internal.h>
#include <Anggur/Math/Vector2.h>

namespace Anggur {
	Window::Window(const Vector2& size, const std::string& title) {
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

		bind();
		initializeGraphicsFunctions();
		initializeEventEmmiter();
	}

	Window::~Window() {
		if (handler) {
			glfwDestroyWindow(handler);
		}
	}

	void Window::setPosition(const Vector2& position) {
		glfwSetWindowPos(handler, position.x, position.y);
	}

	void Window::setSize(const Vector2& size) {
		glfwSetWindowSize(handler, size.x, size.y);
	}

	void Window::setTitle(const std::string& title) {
		glfwSetWindowTitle(handler, title.c_str());

		this->title = title;
	}

	float Window::getAspectRatio() {
		const Vector2& size = getSize();

		return size.y / size.x;
	}

	const Vector2& Window::getPosition() {
		int x, y;

		glfwGetWindowPos(handler, &x, &y);

		position.set(x, y);

		return position;
	}

	const Vector2& Window::getSize() {
		int x, y;

		glfwGetWindowSize(handler, &x, &y);

		size.set(x, y);

		return size;
	}

	const Vector2& Window::getBufferSize() {
		int x, y;

		glfwGetFramebufferSize(handler, &x, &y);

		bufferSize.set(x, y);

		return bufferSize;
	}

	const std::string& Window::getTitle() {
		return title;
	}

	bool Window::isOpen() {
		return !glfwWindowShouldClose(handler);
	}

	void Window::swapBuffers() {
		glfwSwapBuffers(handler);
	}

	WindowHandler* Window::getHandler() {
		return handler;
	}

	void Window::close() {
		glfwSetWindowShouldClose(handler, true);
	}

	void Window::bind() {
		glfwMakeContextCurrent(handler);
	}

	void Window::initializeGraphicsFunctions() {
		bool result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ANGGUR_ASSERT(result, "[Core.Window.load] Failed to load graphic functions");
	}

	void Window::initializeEventEmmiter() {
	}
}
