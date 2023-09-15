#pragma once

#include <string>

namespace Anggur {
	class AudioBuffer {
	public:
		AudioBuffer(const std::string& path);

		~AudioBuffer();

		unsigned int getID() const;

	private:
		unsigned int _id;
	};
}
