#include <Anggur/Window/Event/Common.h>

namespace Anggur 
{
	Event::Event(EventType type, EventGroup group):
		type(type),
		group(group),
		consumed(false) 
	{
	}

	// mouse
	MouseButtonEvent::MouseButtonEvent(EventType type, MouseButton button, ModifierKey modifierKey):
		Event(type, EventGroup::Mouse),
		button(button),
		modifierKey(modifierKey) 
	{
	}

	MousePositionEvent::MousePositionEvent(const Vector2& position):
		Event(EventType::MouseMoved, EventGroup::Mouse),
		position(position) 
	{
	}

	MouseScrollEvent::MouseScrollEvent(const Vector2& direction):
		Event(EventType::MouseScrolled, EventGroup::Mouse),
		direction(direction) 
	{
	}

	// window
	WindowPositionEvent::WindowPositionEvent(const Vector2& position):
		Event(EventType::WindowMoved, EventGroup::Window),
		position(position) 
	{
	}

	WindowSizeEvent::WindowSizeEvent(const Vector2& size):
		Event(EventType::WindowResized, EventGroup::Window),
		size(size) 
	{
	}

	// codepoint
	CodepointEvent::CodepointEvent(uint32_t codepoint):
		Event(EventType::CodepointInput, EventGroup::Codepoint),
		codepoint(codepoint) 
	{
	}

	// frameBuffer
	FrameBufferSizeEvent::FrameBufferSizeEvent(const Vector2& size):
		Event(EventType::FrameBufferResized, EventGroup::FrameBuffer),
		size(size) 
	{
	}
}