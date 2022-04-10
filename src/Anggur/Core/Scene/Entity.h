#pragma once

#include "Anggur/Core/Event/Event.h"
#include "Anggur/Core/Scene/Transform.h"
#include <vector>

namespace Anggur {
	class Entity {
	public:
		virtual void initialize();
		virtual void terminate();

		virtual void update();
		virtual void render();

		virtual void onEvent(Event& event);

	public:
		Transform transform;
		std::vector<Entity*> children;
	};
}