#pragma once

#include "Anggur/Graphics/Renderer.h"
#include "Anggur/OS/Window.h"

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
		Window* _window;
		Input* _input;
		Renderer* _renderer;

		friend class Application;
	};
}
