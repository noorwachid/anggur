#pragma once

#include "Anggur/Graphics/Render/Renderer.h"

namespace Anggur {
	class Object {
	public:
		virtual void initialize() = 0;

		virtual void update(float deltaTime) = 0;

		virtual void draw() = 0;

	protected:
		Renderer* renderer;

		std::vector<Object*> children;
	};
}
