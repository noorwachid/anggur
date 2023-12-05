#pragma once

#include "anggur/os/key.h"
#include "anggur/os/mouse.h"

namespace Anggur
{
	struct WindowPosition
	{
		int x;
		int y;
	};

	struct WindowSize
	{
		unsigned int width;
		unsigned int height;
	};

	class WindowObserver
	{
	public:
		virtual void OnWindowMove(WindowPosition position)
		{
		}

		virtual void OnWindowResize(WindowSize size)
		{
		}

		virtual void OnKeyPress(Key key)
		{
		}

		virtual void OnKeyHold(Key key)
		{
		}

		virtual void OnKeyRelease(Key key)
		{
		}

		virtual void OnType(unsigned int codepoint)
		{
		}

		virtual void OnMousePress(MouseButton button)
		{
		}

		virtual void OnMouseHold(MouseButton button)
		{
		}

		virtual void OnMouseRelease(MouseButton button)
		{
		}

		virtual void OnMouseMove(MousePosition position)
		{
		}

		virtual void OnMouseScroll(MouseScroll scroll)
		{
		}
	};
}
