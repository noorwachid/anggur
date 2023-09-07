#include "Anggur/Audio/AudioDevice.h"
#include "AL/alc.h"
#include <stdexcept>

namespace Anggur
{
	AudioDevice::AudioDevice()
	{
		_device = alcOpenDevice(nullptr);
		if (!_device)
		{
			throw std::runtime_error("failed to open audio device");
		}

		_context = alcCreateContext(_device, nullptr);
		if (!_context)
		{
			throw std::runtime_error("failed to create audio context");
		}

		alcMakeContextCurrent(_context);
	}

	AudioDevice::~AudioDevice()
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(_context);
		alcCloseDevice(_device);
	}
}

