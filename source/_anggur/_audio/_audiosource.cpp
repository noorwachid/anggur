#include "Anggur/Audio/AudioSource.h"
#include "AL/al.h"
#include "AL/alc.h"

namespace Anggur {
	AudioSource::AudioSource() {
		alGenSources(1, &_id);
	}

	AudioSource::~AudioSource() {
		alDeleteSources(1, &_id);
	}

	float AudioSource::getGain() const {
		float value;
		alGetSourcef(_id, AL_GAIN, &value);
		return value;
	}

	float AudioSource::getPitch() const {
		float value;
		alGetSourcef(_id, AL_PITCH, &value);
		return value;
	}

	void AudioSource::setBuffer(AudioBuffer* buffer) {
		if (buffer) {
			alSourcei(_id, AL_BUFFER, buffer->getID());
		}
	}

	void AudioSource::setGain(float gain) {
		alSourcef(_id, AL_GAIN, gain);
	}

	void AudioSource::setPitch(float pitch) {
		alSourcef(_id, AL_PITCH, pitch);
	}

	void AudioSource::play() {
		alSourcePlay(_id);
	}

	void AudioSource::pause() {
		alSourcePause(_id);
	}

	void AudioSource::stop() {
		alSourceStop(_id);
	}
}
