#pragma once

namespace Anggur {
	enum class SamplerFilter {
		nearest = 0x2600,
		linear = 0x2601
	};

	class Texture {
	public:
		static int getMaxSlot();
	};
}
