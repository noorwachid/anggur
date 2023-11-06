#pragma once

#include "anggur/graphics/renderer.h"
#include "anggur/os/window.h"

namespace Anggur {
	class Scene : public WindowObserver {
	public:
		virtual void initialize() {
		}

		virtual void update(float deltaTime) {
		}

		virtual void terminate() {
		}

	protected:
		Window* window;
		Input* input;
		Renderer* renderer;

		friend class Application;
	};
}
