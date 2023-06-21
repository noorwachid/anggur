#pragma once

namespace Anggur {
	enum class SamplerFilter {
		nearest,
		linear
	};

	class TextureSpecification {
	public:
		static int getMaxSlot();
	};
}
