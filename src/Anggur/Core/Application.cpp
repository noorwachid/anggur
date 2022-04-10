#include "Anggur/Core/GraphicFunction.h"
#include "Anggur/Core/Internal.h"
#include "Anggur/Core/Application.h"
#include "Anggur/Core/Scene/SceneManager.h"
#include "Anggur/Core/Event/EventManager.h"
#include "Anggur/Graphic/Renderer.h"

namespace Anggur {

	// public:
	Window* Application::getWindow() {
		return nullptr;
	}

	void Application::run(Scene* scene) {
		initialize();

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

	void Application::initialize() {
		bool ioResult = glfwInit();

		window = new Window();
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
