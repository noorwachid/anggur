#pragma once

#ifdef EMSCRIPTEN
struct ALCdevice_struct;
struct ALCcontext_struct;

using ALCdevice = ALCdevice_struct;
using ALCcontext = ALCcontext_struct;
#else
class ALCdevice;
class ALCcontext;
#endif

namespace Anggur {
	class AudioDevice {
	public:
		AudioDevice();

		~AudioDevice();

	private:
		ALCdevice* _device;
		ALCcontext* _context;
	};
}
