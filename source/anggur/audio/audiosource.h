#pragma once

#include "anggur/audio/audiobuffer.h"

namespace Anggur
{
	class AudioSource
	{
	public:
		AudioSource();

		~AudioSource();

		float GetGain() const;

		float GetPitch() const;

		void SetBuffer(AudioBuffer* buffer);

		void SetGain(float gain);

		void SetPitch(float pitch);

		void Play();

		void Pause();

		void Stop();

	private:
		unsigned int _id;
	};
}
