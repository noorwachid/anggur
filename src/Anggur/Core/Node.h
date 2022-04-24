#pragma once

#include <Anggur/Window/Event/Event.h>
#include <Anggur/Core/Transform.h>
#include <vector>

namespace Anggur {
	class Node {
	public:
		virtual void initialize();
		virtual void terminate();
		virtual void update();
		virtual void onEvent(Event& event);

	public:
		Transform transform;

		std::vector<Node*> children;
	};
}