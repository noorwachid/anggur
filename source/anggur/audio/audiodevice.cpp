#include "anggur/audio/audiodevice.h"
#include "AL/alc.h"
#include <stdexcept>

namespace Anggur {
	AudioDevice::AudioDevice() {
		device = alcOpenDevice(nullptr);
		if (!device) {
			throw std::runtime_error("failed to open audio device");
		}

		context = alcCreateContext(device, nullptr);
		if (!context) {
			throw std::runtime_error("failed to create audio context");
		}

		alcMakeContextCurrent(context);
	}

	AudioDevice::~AudioDevice() {
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}
}
