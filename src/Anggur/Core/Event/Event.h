#pragma once

#include <Anggur/Math/Vector2.h>
#include "Window.h"
#include "Key.h"
#include "Mouse.h"

namespace Anggur {
	enum class EventType {
		keyPressed,
		keyHeld,
		keyReleased,

		mouseMoved,
		mouseScrolled,
		mouseButtonPressed,
		mouseButtonHeld,
		mouseButtonReleased,

		windowMoved,
		windowResized,
		windowMinimized,
		windowMaximized,

		frameBufferResized,
	};

	enum class EventGroup {
		key,
		mouse,
		window,
		frameBuffer,
	};

	struct Event {
		EventType type;
		EventGroup group;
		bool consumed;

		Event(EventType type, EventGroup group);
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

	struct WindowPositionEvent: public Event {
		Vector2 position;

		WindowPositionEvent(const Vector2& position);
	};

	struct WindowSizeEvent: public Event {
		Vector2 size;

		WindowSizeEvent(const Vector2& size);
	};

	struct FrameBufferSizeEvent: public Event {
		Vector2 size;

		FrameBufferSizeEvent(const Vector2& size);
	};
}
