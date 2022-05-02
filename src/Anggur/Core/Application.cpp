#include <Anggur/Core/Application.h>
#include <Anggur/Core/Tree.h>
#include <Anggur/Core/Internal.h>
#include <Anggur/Core/Event/Common.h>
#include <Anggur/Utility/Assert.h>

namespace Anggur {

	// public:
	Window* Application::getWindow() {
		return window;
	}

	void Application::run(Node* rootNode) {
		Tree::setRoot(rootNode);

		while (window->isOpen()) {
			Tree::update();

			window->swapBuffers();

			Window::pollEvents();
		}

		terminate();
	}

	void Application::setVsync(bool enable) {
	}

	void Application::initialize(const Vector2& windowSize, const std::string& windowTitle) {
		bool ioResult = glfwInit();
		ANGGUR_ASSERT(ioResult, "[Core.Application.initialize] Failed to io system");

		window = new Window(windowSize, windowTitle);
		window->bind();
		window->load();

		window->setEventCallback(Tree::emit);
	}

	void Application::terminate() {
		Tree::terminate();
	}

	// private:
	Application::Application() {
	}

	// private:
	Window* Application::window = nullptr;
}
