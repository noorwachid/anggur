#pragma once

#include "Anggur/Math/Vector2.h"
#include "Anggur/OS/InputObserver.h"

namespace Anggur {
	struct WindowMovedEvent : public Event {
		Vector2 position;
	};

	struct WindowResizedEvent : public Event {
		Vector2 size;
	};

	class WindowObserver : public InputObserver {
	public:
		virtual void onWindowMove(WindowMovedEvent& event) {
		}

		virtual void onWindowResize(WindowResizedEvent& event) {
		}
	};
}
