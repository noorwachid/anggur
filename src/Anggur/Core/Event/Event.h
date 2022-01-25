#pragma once

#include <Anggur/Math/Vector.h>
#include "Window.h"
#include "Key.h"
#include "Mouse.h"
#include "Gamepad.h"

namespace Anggur
{
	enum class EventType
	{
		KeyPressed,
		KeyHeld,
		KeyReleased,

		MouseMoved,
		MouseScrolled,
		MouseButtonPressed,
		MouseButtonHeld,
		MouseButtonReleased,
	};

	enum class EventGroup
	{
		Key,
		Mouse,
	};

	struct Event
	{
		EventType type;
		EventGroup group;
		bool consumed;
	};

	struct KeyEvent : public Event
	{
		Scancode scancode;
		Key key;
		Modifier modifier;
	};

	struct MouseMovementEvent : public Event
	{
		Vector position;
		Vector deltaPosition;
	};

	struct MouseScrollEvent : public Event
	{
		Vector direction;
	};

	struct MouseButtonEvent : public Event
	{
		MouseButton button;
		Vector position;
	};
}
