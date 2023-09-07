#include "Anggur/Audio/AudioSource.h"
#include "AL/al.h"
#include "AL/alc.h"

namespace Anggur
{
	AudioSource::AudioSource()
	{
		alGenSources(1, &_id);
	}

	AudioSource::~AudioSource()
	{
		alDeleteSources(1, &_id);
	}

	void AudioSource::SetBuffer(AudioBuffer* buffer)
	{
		if (buffer)
		{
			alSourcei(_id, AL_BUFFER, buffer->GetID());
		}
	}

	void AudioSource::SetGain(float gain)
	{
		alSourcef(_id, AL_GAIN, gain);
	}

	void AudioSource::SetPitch(float pitch)
	{
		alSourcef(_id, AL_PITCH, pitch);
	}

	void AudioSource::Play()
	{
		alSourcePlay(_id);
	}
}

