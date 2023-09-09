#pragma once

namespace Anggur
{
	struct MousePosition
	{
		int x;
		int y;
	};

	struct MouseScroll
	{
		int x;
		int y;
	};

	enum class MouseButton
	{
		Unknown = -1,

		F1 = 0x000,
		F2 = 0x001,
		F3 = 0x002,
		F4 = 0x003,
		F5 = 0x004,
		F6 = 0x005,
		F7 = 0x006,
		F8 = 0x007,
	};
}
