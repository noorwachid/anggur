#include "anggur/application.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "GLFW/glfw3.h"
#include "anggur/audio/audiobuffer.h"
#include "anggur/audio/audiosource.h"
#include "anggur/graphics/driver.h"
#include <fstream>

#ifdef EMSCRIPTEN
#include "emscripten.h"
#endif

namespace Anggur {
	Application::Application(uint32_t width, uint32_t height, const std::string& title)
		: window(width, height, title), scene(nullptr) {
		window.connect([](void* address) -> bool { return loadGraphicsDriver(address); });
		renderer = new Renderer();
	}

	Application::~Application() {
		if (scene) {
			scene->terminate();
			delete scene;
		}

		delete renderer;
	}

	void Application::run() {
#ifdef EMSCRIPTEN
		emscripten_set_main_loop_arg(
			[](void* application) {
				Application& app = *static_cast<Application*>(application);
				if (app._scene)
					app._scene->Update(0.01);
			},
			this, -1, 1
		);
#else
		double previousTime = glfwGetTime();

		while (!window.shouldClose()) {
			double currentTime = glfwGetTime();
			float deltaTime = currentTime - previousTime;
			previousTime = currentTime;

			if (scene)
				scene->update(deltaTime);

			window.swapBuffers();
			window.pollEvents();
		}
#endif
	}

	void Application::stableRun() {
#ifdef EMSCRIPTEN
		emscripten_set_main_loop_arg(
			[](void* application) {
				Application& app = *static_cast<Application*>(application);
				if (app._scene)
					app._scene->Update(0.01);
			},
			this, -1, 1
		);
#else
		const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		float refreshTime = 1.0f / videoMode->refreshRate;
		double previousTime = glfwGetTime();

		while (!window.shouldClose()) {
			double currentTime = glfwGetTime();
			float deltaTime = currentTime - previousTime;
			float sleepTime = refreshTime - deltaTime;
			previousTime = currentTime;

			if (sleepTime > 0.0f) {
				glfwWaitEventsTimeout(sleepTime);
				deltaTime = refreshTime;
			}

			if (scene)
				scene->update(deltaTime);

			window.swapBuffers();
			window.pollEvents();
		}
#endif
	}

	void Application::lazyRun() {
	}
}
