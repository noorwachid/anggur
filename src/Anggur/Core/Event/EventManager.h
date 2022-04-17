#pragma once

#include <Anggur/Core/Window.h>
#include <Anggur/Core/Event/Event.h>

namespace Anggur {
	class EventManager {
	public:
		static void attach(Window* window);

		static void detach(Window* window);

		static void poll();
	};
}