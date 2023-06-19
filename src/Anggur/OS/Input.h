#pragma once

#include "Anggur/OS/InputObserver.h"
#include "Anggur/OS/WindowContext.h"
#include <array>

namespace Anggur {
	class Window;

	// Common Devices
	//
	enum class MouseShape {
		arrow = 0x1,
		iBeam = 0x2,
		crosshair = 0x3,
		hand = 0x4,
		hResize = 0x5,
		vResize = 0x6,
	};

	class Input {
	public:
		void initialize(WindowContext* context);

		bool isKeyPressed(Key key) const;

		bool isKeyHeld(Key key) const;

		bool isKeyReleased(Key key) const;

		const Vector2& getMousePointer() const;

	private:
		void setKeyCallbacks();

		void setMouseCallbacks();

	private:
		Vector2 mousePointer;

		WindowContext* context;
	};
}
