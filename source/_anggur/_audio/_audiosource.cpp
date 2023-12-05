#include "anggur/audio/audiosource.h"
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

	float AudioSource::GetGain() const
	{
		float value;
		alGetSourcef(_id, AL_GAIN, &value);
		return value;
	}

	float AudioSource::GetPitch() const
	{
		float value;
		alGetSourcef(_id, AL_PITCH, &value);
		return value;
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

	void AudioSource::Pause()
	{
		alSourcePause(_id);
	}

	void AudioSource::Stop()
	{
		alSourceStop(_id);
	}
}
