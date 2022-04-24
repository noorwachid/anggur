#pragma once

#include <Anggur/Window/Window.h>
#include <Anggur/Window/Event/Event.h>

namespace Anggur {
	class EventManager {
	public:
		static void attach(Window* window);

		static void detach(Window* window);

		static void poll();
	};
}