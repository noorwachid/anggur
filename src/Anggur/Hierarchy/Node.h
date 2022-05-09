#pragma once

#include <Anggur/Core/Event/Common.h>
#include <vector>

namespace Anggur {
	class Node {
	public:
		virtual void initialize();
		virtual void terminate();
		virtual void update();
		virtual void render();
		virtual void onEvent(Event& event);

	public:
		Vector2 position;
		Vector2 scale;
		float rotation;

		std::vector<Node*> children;
	};
}