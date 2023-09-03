#include "Anggur/Application.h"
#include "Anggur/Graphics/Pointer.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "sndfile.h"

#ifdef EMSCRIPTEN
#include "emscripten.h"
#endif

namespace Anggur
{
	Application::Application(uint32_t width, uint32_t height) : _window(width, height), _scene(nullptr)
	{
		_window.Connect([](void* address) -> bool { return ConnectPointer(address); });
		_renderer = new Renderer();
	}

	Application::~Application()
	{
		if (_scene)
			delete _scene;

		delete _renderer;
	}

	void Application::Run()
	{
		ALCdevice* device = alcOpenDevice(NULL);
		if (!device)
		{
			// Handle error
		}

		ALCcontext* context = alcCreateContext(device, NULL);
		if (!context)
		{
			// Handle error
		}

		alcMakeContextCurrent(context);

		// Create an OpenAL source and buffer for audio playback
		ALuint source;
		ALuint buffer;

		alGenSources(1, &source);
		alGenBuffers(1, &buffer);

		SF_INFO sfinfo;
		SNDFILE* sndfile = sf_open("your_wav_file.wav", SFM_READ, &sfinfo);

		if (!sndfile)
		{
			std::cerr << "Error opening WAV file" << std::endl;
		}

		ALsizei num_samples = sfinfo.frames * sfinfo.channels;
		ALshort* buffer_data = new ALshort[num_samples];
		sf_readf_short(sndfile, buffer_data, num_samples);

		alBufferData(
			buffer, (sfinfo.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, buffer_data,
			num_samples * sizeof(ALshort), sfinfo.samplerate
		);

		sf_close(sndfile);
		delete[] buffer_data;

		alSourcei(source, AL_BUFFER, buffer);
		alSourcef(source, AL_GAIN, 1.0f);  // Set volume (1.0 = full volume)
		alSourcef(source, AL_PITCH, 1.0f); // Set pitch (1.0 = normal pitch)

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

		// Play the audio source
		alSourcePlay(source);

		while (!_window.ShouldClose())
		{
			if (_scene)
				_scene->Update(0.016f);

			_window.SwapBuffers();
			_window.PollEvents();
		}
		#endif

		// Cleanup OpenAL (placed after the main loop)
		alDeleteSources(1, &source);
		alDeleteBuffers(1, &buffer);

		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}
}
