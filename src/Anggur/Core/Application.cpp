#include <Anggur/Core/Application.h>
#include <Anggur/Core/Tree.h>
#include <Anggur/Window/Internal.h>
#include <Anggur/Window/Event/EventManager.h>
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

			EventManager::poll();
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

		EventManager::attach(window);
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
