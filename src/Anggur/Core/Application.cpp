#include "Anggur/Core/GraphicFunction.h"
#include "Anggur/Core/Internal.h"
#include "Anggur/Core/Log.h"
#include "Anggur/Core/Application.h"
#include "Anggur/Core/Scene/SceneManager.h"
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

			window->swapBuffers();

			glfwPollEvents();
		}

		SceneManager::destroy();

		terminate();
	}

	void Application::setVsync(bool enable) {

	}

	void Application::initialize() {
		bool ioResult = glfwInit();
		ANGGUR_ASSERT(ioResult, "Failed to initialize IO");

		window = new Window();
		window->bind();

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
