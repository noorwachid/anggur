#pragma once

#include "Anggur/IO/WindowSystem.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/Clock.h"
#include "Anggur/Graphics/Renderer.h"
#include "Entity.h"

using namespace Anggur;

class CoreSystem {
public:
	CoreSystem() {
	}

	void initialize() {
		previousTime = clock.getElapsed();
	}

	void run() {
        initialize();
        
		while (window.isOpen()) {
			windowSystem.pollEvents();

			update();

			window.update();
		}
	}

	template <typename E>
	void setRootEntity() {
		entity.reset(new E(window, renderer));
		entity->initialize();
	}

	void update() {
		float currentTime = clock.getElapsed();
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		if (entity)
			entity->update(deltaTime);
	}

private:
	// Window system
	WindowSystem windowSystem;
	Window window;

	// Rendering system
	Renderer renderer;

	// Hierarchy system
	std::shared_ptr<Entity> entity = nullptr;

	// Time system
	Clock clock;
	float previousTime = 0;
};