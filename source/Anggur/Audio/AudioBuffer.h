#pragma once

#include <string>

namespace Anggur
{
	class AudioBuffer
	{
	public:
		AudioBuffer(const std::string& path);

		~AudioBuffer();

		unsigned int GetID() const;

	private:
		unsigned int _id;
	};
}

