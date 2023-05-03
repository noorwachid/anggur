#pragma once

#include "Anggur/OS/InputEvent.h"
#include "Anggur/OS/WindowContext.h"
#include <array>

namespace Anggur
{
	class Window;

	// Common Devices
	//
	enum class MouseShape
	{
		Arrow = 0x1,
		I = 0x2,
		Crosshair = 0x3,
		Hand = 0x4,
		HorizontalResize = 0x5,
		VerticalResize = 0x6,
	};

	class Input
	{
	public:
		void Initialize(WindowContext* context);

		bool IsKeyboardPressed(Key key) const;

		bool IsKeyboardHeld(Key key) const;

		bool IsKeyboardReleased(Key key) const;

		const Vector2& GetMousePosition() const;

	private:
		void SetKeyCallbacks();

		void SetMouseCallbacks();

	private:
		Vector2 mousePosition;

		WindowContext* context;
	};
}
