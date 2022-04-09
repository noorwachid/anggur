#pragma once

#include "Anggur/Core/Event/Event.h"

namespace Anggur {
	class Scene {
	public:
		virtual void onAttach();
		virtual void onDetach();

		virtual void onUpdate();

		virtual void onEvent(Event& event);
	};
}
