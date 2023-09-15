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

struct WavHeader {
	char chunkId[4];
	uint32_t chunkSize;
	char format[4];
	char subchunk1Id[4];
	uint32_t subchunk1Size;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
	char subchunk2Id[4];
	uint32_t subchunk2Size;
};

namespace Anggur {
	Application::Application(uint32_t width, uint32_t height, const std::string& title)
		: _window(width, height, title), _scene(nullptr) {
		_window.connect([](void* address) -> bool { return loadGraphicsDriver(address); });
		_renderer = new Renderer();
	}

	Application::~Application() {
		if (_scene) {
			_scene->terminate();
			delete _scene;
		}

		delete _renderer;
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

		while (!_window.shouldClose()) {
			double currentTime = glfwGetTime();
			float deltaTime = currentTime - previousTime;
			previousTime = currentTime;

			if (_scene)
				_scene->update(deltaTime);

			_window.swapBuffers();
			_window.pollEvents();
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

		while (!_window.shouldClose()) {
			double currentTime = glfwGetTime();
			float deltaTime = currentTime - previousTime;
			float sleepTime = refreshTime - deltaTime;
			previousTime = currentTime;

			if (sleepTime > 0.0f) {
				glfwWaitEventsTimeout(sleepTime);
				deltaTime = refreshTime;
			}

			if (_scene)
				_scene->update(deltaTime);

			_window.swapBuffers();
			_window.pollEvents();
		}

#endif
	}

	void Application::lazyRun() {
	}
}
