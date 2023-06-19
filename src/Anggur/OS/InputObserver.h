#pragma once

#include "Anggur/Core/Observer.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/OS/Key.h"
#include "Anggur/OS/Mouse.h"

namespace Anggur {
	struct KeyEvent : Event {
		ModifierKey modifierKey = ModifierKey::none;
		Key key = Key::unknown;
	};

	struct TypeEvent : Event {
		unsigned int codepoint;
	};

	struct MouseButtonEvent : Event {
		ModifierKey modifierKey = ModifierKey::none;
		MouseButton button = MouseButton::unknown;
	};

	struct MousePointerEvent : Event {
		Vector2 position;
	};

	struct ScrollEvent : Event {
		Vector2 direction;
	};

	class InputObserver {
	public:
		virtual void onKeyPress(KeyEvent& event) {
		}

		virtual void onKeyHold(KeyEvent& event) {
		}

		virtual void onKeyRelease(KeyEvent& event) {
		}

		virtual void onType(TypeEvent& event) {
		}

		virtual void onMouseButtonPress(MouseButtonEvent& event) {
		}

		virtual void onMouseButtonHold(MouseButtonEvent& event) {
		}

		virtual void onMouseButtonRelease(MouseButtonEvent& event) {
		}

		virtual void onMousePointerMove(MousePointerEvent& event) {
		}

		virtual void onScroll(ScrollEvent& event) {
		}
	};
}
