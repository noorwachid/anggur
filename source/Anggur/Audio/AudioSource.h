#pragma once

#include "Anggur/Audio/AudioBuffer.h"

namespace Anggur {
	class AudioSource {
	public:
		AudioSource();

		~AudioSource();

		float getGain() const;

		float getPitch() const;

		void setBuffer(AudioBuffer* buffer);

		void setGain(float gain);

		void setPitch(float pitch);

		void play();

		void pause();

		void stop();

	private:
		unsigned int _id;
	};
}
