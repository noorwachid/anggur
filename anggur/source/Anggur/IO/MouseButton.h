#pragma once

namespace Anggur {
	constexpr int maxMouseButtonValue = 8;

	enum class MouseButton: int {
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
