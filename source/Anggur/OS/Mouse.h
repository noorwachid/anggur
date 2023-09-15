#pragma once

namespace Anggur {
	struct MousePosition {
		float x;
		float y;
	};

	struct MouseScroll {
		float x;
		float y;
	};

	enum class MouseButton {
		unknown = -1,

		f1 = 0x000,
		f2 = 0x001,
		f3 = 0x002,
		f4 = 0x003,
		f5 = 0x004,
		f6 = 0x005,
		f7 = 0x006,
		f8 = 0x007,
	};
}
