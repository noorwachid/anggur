#pragma once

#include "anggur/os/key.h"
#include "anggur/os/mouse.h"

namespace Anggur {
	struct WindowPosition {
		int x;
		int y;
	};

	struct WindowSize {
		unsigned int width;
		unsigned int height;
	};

	class WindowObserver {
	public:
		virtual void onWindowMove(WindowPosition position) {
		}

		virtual void onWindowResize(WindowSize size) {
		}

		virtual void onKeyPress(Key key) {
		}

		virtual void onKeyHold(Key key) {
		}

		virtual void onKeyRelease(Key key) {
		}

		virtual void onType(unsigned int codepoint) {
		}

		virtual void onMousePress(MouseButton button) {
		}

		virtual void onMouseHold(MouseButton button) {
		}

		virtual void onMouseRelease(MouseButton button) {
		}

		virtual void onMouseMove(MousePosition position) {
		}

		virtual void onMouseScroll(MouseScroll scroll) {
		}
	};
}
