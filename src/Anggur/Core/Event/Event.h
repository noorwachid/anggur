#pragma once

#include <Anggur/Core/Event/Window.h>
#include <Anggur/Core/Event/Key.h>
#include <Anggur/Core/Event/Mouse.h>
#include <Anggur/Math/Vector2.h>

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

		codepointInput,

		frameBufferResized,
	};

	enum class EventGroup {
		// hardware
		key,
		mouse,
		window,

		// software
		codepoint,
		frameBuffer,
	};

	struct Event {
		EventType type;
		EventGroup group;
		bool consumed;

		Event(EventType type, EventGroup group);
	};

	struct KeyEvent: public Event {
		Key key;
		ModifierKey modifierKey;

		KeyEvent(EventType type, Key key, ModifierKey modifierKey);
	};

	struct MouseButtonEvent: public Event {
		MouseButton button;
		ModifierKey modifierKey;

		MouseButtonEvent(EventType type, MouseButton button, ModifierKey modifierKey);
	};

	struct MousePositionEvent: public Event {
		Vector2 position;

		MousePositionEvent(const Vector2& position);
	};

	struct MouseScrollEvent : public Event {
		Vector2 direction;

		MouseScrollEvent(const Vector2& direction);
	};

	struct WindowPositionEvent: public Event {
		Vector2 position;

		WindowPositionEvent(const Vector2& position);
	};

	struct WindowSizeEvent: public Event {
		Vector2 size;

		WindowSizeEvent(const Vector2& size);
	};

	struct CodepointEvent: public Event {
		uint32_t codepoint;

		CodepointEvent(uint32_t codepoint);
	};

	struct FrameBufferSizeEvent: public Event {
		Vector2 size;

		FrameBufferSizeEvent(const Vector2& size);
	};
}
