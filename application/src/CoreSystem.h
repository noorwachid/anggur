#pragma once

#include "Anggur/IO/WindowSystem.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/Clock.h"
#include "Anggur/Graphics/Renderer.h"
#include "Entity.h"

using namespace Anggur;

class CoreSystem {
public:
	void initialize() {
		previousTime = clock.getElapsed();
	}

	void run() {
        initialize();
        
		while (window->isOpen()) {
			windowSystem.pollEvents();

			update();

			window->update();
		}
	}

	void setWindow(const std::shared_ptr<Window>& newWindow) {
		window = newWindow;
		renderer = std::make_shared<Renderer>();
	}

	void setRootEntity(const std::shared_ptr<Entity>& newEntity) {
		entity = newEntity;
		entity->window = window;
		entity->renderer = renderer;
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
	std::shared_ptr<Window> window = nullptr;

	// Hierarchy system
	std::shared_ptr<Entity> entity = nullptr;

	// Rendering system
	std::shared_ptr<Renderer> renderer = nullptr;

	// Time system
	Clock clock;
	float previousTime = 0;
};