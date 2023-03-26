#pragma once

#include "Anggur/Event.h"
#include "Anggur/Integral.h"
#include "Anggur/IO/Key.h"
#include "Anggur/IO/MouseButton.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur
{
	struct KeyboardPressedEvent : Event
	{
		Key Key = Key::Unknown;
	};

	struct KeyboardHeldEvent : Event
	{
		Key Key = Key::Unknown;
	};

	struct KeyboardReleasedEvent : Event
	{
		Key Key = Key::Unknown;
	};

	struct KeyboardTypedEvent : public Event
	{
		uint Codepoint;
	};

	struct MouseMovedEvent : public Event
	{
		Vector2 Position;
	};

	struct MousePressedEvent : Event
	{
		MouseButton Button = MouseButton::Unknown;
	};

	struct MouseHeldEvent : Event
	{
		Key Key = Key::Unknown;
	};

	struct MouseReleasedEvent : Event
	{
		Key Key = Key::Unknown;
	};

	struct MouseScrolledEvent : public Event
	{
		Vector2 Direction;
	};

	class InputEventListener 
	{
	public:
		virtual void OnKeyboardPress(KeyboardPressedEvent& event) {}

		virtual void OnKeyboardHold(KeyboardHeldEvent& event) {}

		virtual void OnKeyboardRelease(KeyboardReleasedEvent& event) {}

		virtual void OnKeyboardType(KeyboardTypedEvent& event) {}

		virtual void OnMousePress(MousePressedEvent& event) {}

		virtual void OnMouseHold(MouseHeldEvent& event) {}

		virtual void OnMouseRelease(MouseReleasedEvent& event) {}

		virtual void OnMouseMove(MouseMovedEvent& event) {}

		virtual void OnMouseScroll(MouseScrolledEvent& event) {}
	};
}
