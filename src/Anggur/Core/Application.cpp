#include "Anggur/Core/GraphicFunction.h"
#include "Anggur/Core/Internal.h"
#include "Anggur/Core/Application.h"
#include "Anggur/Core/Scene/SceneManager.h"
#include "Anggur/Core/Event/EventManager.h"
#include "Anggur/Graphic/Renderer.h"

namespace Anggur {

	// public:
	Window* Application::getWindow() {
		return window;
	}

	void Application::run(Scene* scene, const Vector2& windowSize, const std::string& windowTitle) {
		initialize(windowSize, windowTitle);

		SceneManager::set(scene);

		while (window->isOpen()) {
			SceneManager::update();
			Renderer::render();

			window->swapBuffers();

			EventManager::poll();
		}

		SceneManager::terminate();

		terminate();
	}

	void Application::setVsync(bool enable) {
	}

	void Application::initialize(const Vector2& windowSize, const std::string& windowTitle) {
		bool ioResult = glfwInit();

		window = new Window(windowSize, windowTitle);
		window->bind();

		EventManager::attach(window);

		GraphicFunction::load();

		Renderer::initialize();
	}

	void Application::terminate() {

	}

	// private:
	Application::Application() {

	}


	// private:
	Window* Application::window = nullptr;

}
