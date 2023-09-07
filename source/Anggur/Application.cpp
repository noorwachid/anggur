#include "Anggur/Application.h"
#include "Anggur/Audio/AudioBuffer.h"
#include "Anggur/Audio/AudioSource.h"
#include "Anggur/Graphics/Driver.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "GLFW/glfw3.h"
#include <fstream>

#ifdef EMSCRIPTEN
#include "emscripten.h"
#endif

struct WavHeader 
{
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

namespace Anggur
{
	Application::Application(uint32_t width, uint32_t height) : _window(width, height), _scene(nullptr)
	{
		_window.Connect([](void* address) -> bool { return LoadGraphicsDriver(address); });
		_renderer = new Renderer();
	}

	Application::~Application()
	{
		if (_scene)
		{
			_scene->Terminate();
			delete _scene;
		}

		delete _renderer;
	}

	void Application::Run()
	{
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

		while (!_window.ShouldClose())
		{
			if (_scene)
				_scene->Update(0.016f);

			_window.SwapBuffers();
			_window.PollEvents();
		}

		#endif
	}
}
