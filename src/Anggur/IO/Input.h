#pragma once

#include "Anggur/IO/WindowContext.h"
#include "InputEvent.h"
#include <array>

namespace Anggur
{
	class Window;

	class InputDevice
	{
      public:
        virtual void Initialize(WindowContext* context);

      protected:
        WindowContext* context;

        friend class Window;
	};

	// Common Devices

	class Keyboard : public InputDevice
	{
	  public:
        virtual void Initialize(WindowContext* context) override;

		bool IsKeyPressed(Key key) const;

		bool IsKeyReleased(Key key) const;

		bool IsKeyDown(Key key) const;

		bool IsKeyUp(Key key) const;
	};

	enum class MouseCursorShape
	{
		Arrow = 0x1,
		I = 0x2,
		Crosshair = 0x3,
		Hand = 0x4,
		HorizontalResize = 0x5,
		VerticalResize = 0x6,
	};

	class Mouse : public InputDevice
	{
      public:
        virtual void Initialize(WindowContext* context) override;

	};
}
