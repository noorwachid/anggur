#pragma once

#include "Anggur/Event.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur
{
	struct WindowResizedEvent : public Event
	{
		Vector2 size;

		WindowResizedEvent(const Vector2& newSize) : Event(GetID<WindowResizedEvent>()), size(newSize)
		{
		}
	};

	struct WindowMovedEvent : public Event
	{
		Vector2 position;

		WindowMovedEvent(const Vector2& newPosition) : Event(GetID<WindowMovedEvent>()), position(newPosition)
		{
		}
	};
}
