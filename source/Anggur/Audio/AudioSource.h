#pragma once

#include "Anggur/Audio/AudioBuffer.h"

namespace Anggur
{
	class AudioSource
	{
	public:
		AudioSource();

		~AudioSource();

		void SetBuffer(AudioBuffer* buffer);

		void SetGain(float gain);

		void SetPitch(float pitch);

		void Play();

	private:
		unsigned int _id;
	};
}
