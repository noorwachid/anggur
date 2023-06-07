#pragma once

#include "Anggur/Core/Observer.h"
#include "Anggur/OS/Key.h"
#include "Anggur/OS/Mouse.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur
{
	struct KeyEvent : Event
	{
		ModifierKey modifierKey = ModifierKey::None;
		Key key = Key::Unknown;
	};

	struct TextEvent : Event
	{
		unsigned int codepoint;
	};

	struct MouseButtonEvent : Event
	{
		MouseButton button = MouseButton::Unknown;
	};

	struct MousePointerEvent : Event
	{
		Vector2 position;
	};

	struct ScrollEvent : Event
	{
		Vector2 direction;
	};

	class InputObserver 
	{
	public:
		virtual void OnKeyPress(KeyEvent& event) {}

		virtual void OnKeyHold(KeyEvent& event) {}

		virtual void OnKeyRelease(KeyEvent& event) {}

		virtual void OnText(TextEvent& event) {}

		virtual void OnMouseButtonPress(MouseButtonEvent& event) {}

		virtual void OnMouseButtonHold(MouseButtonEvent& event) {}

		virtual void OnMouseButtonRelease(MouseButtonEvent& event) {}

		virtual void OnMousePointerMove(MousePointerEvent& event) {}

		virtual void OnScroll(ScrollEvent& event) {}
	};
}
