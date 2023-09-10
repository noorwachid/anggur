#pragma once

#include "Anggur/OS/Key.h"

namespace Anggur
{
	class Input
	{
	public:
		ModifierKey GetModifierKey() const;

	private:
		ModifierKey _modifierKey;

	private:
		friend class Window;
	};
}
