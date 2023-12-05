#pragma once

#include "anggur/os/key.h"

namespace Anggur
{
	class Input
	{
	public:
		ModifierKey GetModifierKey() const;

	private:
		ModifierKey _modifierKey = ModifierKey::None;

	private:
		friend class Window;
	};
}
