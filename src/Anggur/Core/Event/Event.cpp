#include "Anggur/Core/Event/Event.h"

namespace Anggur {
	Event::Event(EventType type, EventGroup group):
		type(type),
		group(group),
		consumed(false) {
	}

	// window
	WindowPositionEvent::WindowPositionEvent(const Vector2& position):
		Event(EventType::windowMoved, EventGroup::window),
		position(position) {
	}

	WindowSizeEvent::WindowSizeEvent(const Vector2& size):
		Event(EventType::windowResized, EventGroup::window),
		size(size) {
	}

	// frameBuffer
	FrameBufferSizeEvent::FrameBufferSizeEvent(const Vector2& size):
		Event(EventType::frameBufferResized, EventGroup::frameBuffer),
		size(size) {
	}
}