#pragma once

#include "Anggur/Common/Event.h"
#include "Anggur/Common/Integral.h"
#include "Anggur/IO/Key.h"
#include "Anggur/IO/MouseButton.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur
{
	struct KeyboardPressedEvent : Event
	{
		Key key = Key::Unknown;
	};

	struct KeyboardHeldEvent : Event
	{
		Key key = Key::Unknown;
	};

	struct KeyboardReleasedEvent : Event
	{
		Key key = Key::Unknown;
	};

	struct KeyboardTypedEvent : public Event
	{
		uint codepoint;
	};

	struct MouseMovedEvent : public Event
	{
		Vector2 position;
	};

	struct MousePressedEvent : Event
	{
		MouseButton button = MouseButton::Unknown;
	};

	struct MouseHeldEvent : Event
	{
		MouseButton button = MouseButton::Unknown;
	};

	struct MouseReleasedEvent : Event
	{
		MouseButton button = MouseButton::Unknown;
	};

	struct MouseScrolledEvent : public Event
	{
		Vector2 direction;
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
