#include "Anggur/OS/Window.h"
#include "Anggur/Core/Instrumentation.h"
#include "Anggur/OS/Internal.h"
#include <cassert>

namespace Anggur {
	Window::Window(const Vector2& newSize, const std::string& newTitle, const WindowContextLoader& contextLoader)
		: size(newSize), title(newTitle) {
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

		bindGraphicsContext();

		glfwSwapInterval(1);

		bindGraphicsAPI();

		assert(contextLoader && "Context loader is empty");
		bool contextLoaded = contextLoader((void*)glfwGetProcAddress);
		assert(contextLoaded && "Context is failed to load");

		glfwSetWindowSizeCallback(context, [](GLFWwindow* context, int width, int height) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			Vector2 size(width, height);
			WindowResizedEvent event;
			event.size = size;

			if (window->observer)
				window->observer->onWindowResize(event);

			window->size = size;
		});

		glfwSetFramebufferSizeCallback(context, [](GLFWwindow* context, int width, int height) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			Vector2 size(width, height);
			WindowResizedEvent event;
			event.size = size;

			window->frameBufferSize = size;
		});

		glfwSetWindowPosCallback(context, [](GLFWwindow* context, int x, int y) {
			auto window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			Vector2 position(x, y);
			WindowMovedEvent event;
			event.position = position;

			if (window->observer)
				window->observer->onWindowMove(event);

			window->position = position;
		});

		int fbx, fby;
		glfwGetFramebufferSize(context, &fbx, &fby);
		frameBufferSize.set(fbx, fby);

		input.initialize(context);
	}

	Window::~Window() {
		glfwDestroyWindow(context);
	}

	WindowContext* Window::getContext() {
		return context;
	}

	float Window::getAspectRatio() {
		return size.x / size.y;
	}

	float Window::getScale() {
		return frameBufferSize.x / size.x;
	}

	const Vector2& Window::getPosition() {
		return position;
	}

	const Vector2& Window::getSize() {
		return size;
	}

	const Vector2& Window::getFrameBufferSize() {
		return frameBufferSize;
	}

	const std::string& Window::getTitle() {
		return title;
	}

	void Window::setPosition(const Vector2& position) {
		glfwSetWindowPos(context, position.x, position.y);
	}

	void Window::setSize(const Vector2& size) {
		glfwSetWindowSize(context, size.x, size.y);
	}

	void Window::setTitle(const std::string& newTitle) {
		glfwSetWindowTitle(context, newTitle.c_str());
		title = newTitle;
	}

	bool Window::isOpen() {
		return !glfwWindowShouldClose(context);
	}

	void Window::close() {
		glfwSetWindowShouldClose(context, true);
	}

	void Window::update() {
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		bindGraphicsAPI();
		swapFrameBuffers();
	}

	void Window::bindGraphicsAPI() {
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		glfwMakeContextCurrent(context);
	}

	void Window::swapFrameBuffers() {
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		glfwSwapBuffers(context);
	}

	void Window::bindGraphicsContext() {
		glfwSetWindowUserPointer(context, this);
	}
}
