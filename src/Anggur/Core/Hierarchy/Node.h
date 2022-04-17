#pragma once

#include <Anggur/Core/Event/Event.h>
#include <Anggur/Core/Hierarchy/Transform.h>
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