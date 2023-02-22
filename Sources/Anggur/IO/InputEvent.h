#pragma once

#include "Anggur/Event.h"
#include "Anggur/IO/Key.h"
#include "Anggur/IO/MouseButton.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur
{
	struct KeyEvent : Event
	{
		Key key = Key::Unknown;

		KeyEvent(ID newType, Key newKey) : Event(newType), key(newKey)
		{
		}
	};

	struct KeyPressedEvent : KeyEvent
	{
		KeyPressedEvent(Key newKey) : KeyEvent(GetID<KeyPressedEvent>(), newKey)
		{
		}
	};

	struct KeyHeldEvent : KeyEvent
	{
		KeyHeldEvent(Key newKey) : KeyEvent(GetID<KeyHeldEvent>(), newKey)
		{
		}
	};

	struct KeyReleasedEvent : KeyEvent
	{
		KeyReleasedEvent(Key newKey) : KeyEvent(GetID<KeyReleasedEvent>(), newKey)
		{
		}
	};

	struct KeyComposedEvent : public Event
	{
		uint32_t codepoint;

		KeyComposedEvent(uint32_t newCodepoint) : Event(GetID<KeyComposedEvent>()), codepoint(newCodepoint)
		{
		}
	};

	struct MouseEvent : public Event
	{
		MouseButton button = MouseButton::Unknown;

		MouseEvent(ID newType, MouseButton newButton) : Event(newType), button(newButton)
		{
		}
	};

	struct MousePressedEvent : public MouseEvent
	{
		MousePressedEvent(MouseButton newButton) : MouseEvent(GetID<MousePressedEvent>(), newButton)
		{
		}
	};

	struct MouseHeldEvent : public MouseEvent
	{
		MouseHeldEvent(MouseButton newButton) : MouseEvent(GetID<MouseHeldEvent>(), newButton)
		{
		}
	};

	struct MouseReleasedEvent : public MouseEvent
	{
		MouseReleasedEvent(MouseButton newButton) : MouseEvent(GetID<MouseReleasedEvent>(), newButton)
		{
		}
	};

	struct MouseMovedEvent : public Event
	{
		Vector2 position;

		MouseMovedEvent(const Vector2& newPosition) : Event(GetID<MouseMovedEvent>()), position(newPosition)
		{
		}
	};

	struct MouseScrolledEvent : public Event
	{
		Vector2 direction;

		MouseScrolledEvent(const Vector2& newDirection) : Event(GetID<MouseScrolledEvent>()), direction(newDirection)
		{
		}
	};
}
