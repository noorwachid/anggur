#pragma once

#include "anggur/os/key.h"

namespace Anggur {
	class Input {
	public:
		ModifierKey getModifierKey() const;

	private:
		ModifierKey _modifierKey = ModifierKey::none;

	private:
		friend class Window;
	};
}
