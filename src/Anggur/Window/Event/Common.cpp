#include <Anggur/Window/Event/Common.h>

namespace Anggur {
	Event::Event(EventType type, EventGroup group):
		type(type),
		group(group),
		consumed(false) {
	}

	// mouse
	MouseButtonEvent::MouseButtonEvent(EventType type, MouseButton button, ModifierKey modifierKey):
		Event(type, EventGroup::mouse),
		button(button),
		modifierKey(modifierKey) {
	}

	MousePositionEvent::MousePositionEvent(const Vector2& position):
		Event(EventType::mouseMoved, EventGroup::mouse),
		position(position) {
	}

	MouseScrollEvent::MouseScrollEvent(const Vector2& direction):
		Event(EventType::mouseScrolled, EventGroup::mouse),
		direction(direction) {
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

	// codepoint
	CodepointEvent::CodepointEvent(uint32_t codepoint):
		Event(EventType::codepointInput, EventGroup::codepoint),
		codepoint(codepoint) {
	}

	// frameBuffer
	FrameBufferSizeEvent::FrameBufferSizeEvent(const Vector2& size):
		Event(EventType::frameBufferResized, EventGroup::frameBuffer),
		size(size) {
	}
}