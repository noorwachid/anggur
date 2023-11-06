#include "anggur/audio/audiosource.h"
#include "AL/al.h"
#include "AL/alc.h"

namespace Anggur {
	AudioSource::AudioSource() {
		alGenSources(1, &id);
	}

	AudioSource::~AudioSource() {
		alDeleteSources(1, &id);
	}

	float AudioSource::getGain() const {
		float value;
		alGetSourcef(id, AL_GAIN, &value);
		return value;
	}

	float AudioSource::getPitch() const {
		float value;
		alGetSourcef(id, AL_PITCH, &value);
		return value;
	}

	void AudioSource::setBuffer(AudioBuffer* buffer) {
		if (buffer) {
			alSourcei(id, AL_BUFFER, buffer->getID());
		}
	}

	void AudioSource::setGain(float gain) {
		alSourcef(id, AL_GAIN, gain);
	}

	void AudioSource::setPitch(float pitch) {
		alSourcef(id, AL_PITCH, pitch);
	}

	void AudioSource::play() {
		alSourcePlay(id);
	}

	void AudioSource::pause() {
		alSourcePause(id);
	}

	void AudioSource::stop() {
		alSourceStop(id);
	}
}
