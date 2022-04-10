#include "Anggur/Core/Log.h"
#include "Anggur/Core/Window.h"
#include "Anggur/Core/Application.h"
#include "Anggur/Core/IO.h"
#include "Anggur/Core/Internal.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Graphic/Image.h"

namespace Anggur {
	Window::Window(const Vector2& size, const std::string& title) {
		this->size = size;
		this->title = title;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef ANGGUR_OS_APPLE
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		handler = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
		ANGGUR_ASSERT(handler, "Failed to create a window");
	}

	Window::~Window() {
		if (handler) {
			delete handler;
			handler = nullptr;
		}
	}

	void Window::setPosition(const Vector2& position) {
		glfwSetWindowPos(handler, position.x, position.y);

		this->position = position;
	}

	void Window::setSize(const Vector2& size) {
		glfwSetWindowSize(handler, size.x, size.y);

		this->size = size;
	}

	void Window::setTitle(const std::string& title) {
		glfwSetWindowTitle(handler, title.c_str());

		this->title = title;
	}

	float Window::getRatio() {
		return position.x / position.y;
	}

	Vector2 Window::getPosition() {
		int x, y;

		glfwGetWindowPos(handler, &x, &y);

		position.x = x;
		position.y = y;

		return position;
	}

	Vector2 Window::getSize() {
		int x, y;

		glfwGetWindowSize(handler, &x, &y);

		size.x = x;
		size.y = y;

		return size;
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
}
