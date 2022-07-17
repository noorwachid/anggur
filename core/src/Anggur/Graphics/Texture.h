#pragma once

#include <Anggur/Graphics/Image.h>

namespace Anggur 
{
	enum class SamplerFilter 
	{
		Nearest = 0x2600,
		Linear = 0x2601
	};

	class Texture 
	{
	public:
		static int GetMaxSlot();
	};
}