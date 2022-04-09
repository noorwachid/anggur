#pragma once

#include <Anggur/Math/Vector2.h>
#include "Window.h"
#include "Key.h"
#include "Mouse.h"

namespace Anggur {
	enum class EventType {
		KeyPressed,
		KeyHeld,
		KeyReleased,

		MouseMoved,
		MouseScrolled,
		MouseButtonPressed,
		MouseButtonHeld,
		MouseButtonReleased,
	};

	enum class EventGroup {
		Key,
		Mouse,
	};

	struct Event {
		EventType type;
		EventGroup group;
		bool consumed;
	};

	struct KeyEvent : public Event {
		Scancode scancode;
		Key key;
		Modifier modifier;
	};

	struct MouseMovementEvent : public Event {
		Vector2 position;
		Vector2 deltaPosition;
	};

	struct MouseScrollEvent : public Event {
		Vector2 direction;
	};

	struct MouseButtonEvent : public Event {
		MouseButton button;
		Vector2 position;
	};
}
