#pragma once

#include "Anggur/Event.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/IO/InputEvent.h"

namespace Anggur
{
	struct WindowMovedEvent : public Event
	{
		Vector2 position;
	};

	struct WindowResizedEvent : public Event
	{
		Vector2 size;
	};

	class WindowEventListener : public InputEventListener
	{
	public:
		virtual void OnWindowMove(WindowMovedEvent& event) {}

		virtual void OnWindowResize(WindowResizedEvent& event) {}
	};
}