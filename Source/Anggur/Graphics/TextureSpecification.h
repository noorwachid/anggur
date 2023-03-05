#pragma once

namespace Anggur
{
	enum class SamplerFilter
	{
		Nearest = 0x2600,
		Linear = 0x2601
	};

	class TextureSpecification
	{
	public:
		static int GetMaxSlot();
	};
}
