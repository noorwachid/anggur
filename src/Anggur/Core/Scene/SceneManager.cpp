#include "Anggur/Core/Scene/SceneManager.h"
#include "Anggur/Core/Log.h"

namespace Anggur {
	// public:
	void SceneManager::initialize() {
		traverseAndInitialize(scene);
	}

	void SceneManager::terminate() {
		traverseAndTerminate(scene);
	}

	Scene* SceneManager::get() {
		return scene;
	}

	void SceneManager::set(Scene* scene) {
		terminate();

		SceneManager::scene = scene;

		initialize();
	}

	void SceneManager::update() {
		traverseAndUpdate(scene);
	}

	void SceneManager::emit(Event& event) {
		traverseAndEmit(scene, event);
	}

	void SceneManager::traverseAndInitialize(Entity* entity) {
		if (entity) {
			// BFS traversal
			entity->initialize();

			for (Entity* childEntity: entity->children) {
				traverseAndInitialize(childEntity);
			}
		}
	}

	void SceneManager::traverseAndTerminate(Entity* entity) {
		if (entity) {
			// DFS traversal
			for (Entity* childEntity: entity->children) {
				traverseAndTerminate(childEntity);
			}

			entity->terminate();
			delete entity;
			entity = nullptr;
		}
	}

	void SceneManager::traverseAndUpdate(Entity* entity) {
		if (entity) {
			// BFS traversal
			entity->update();
			entity->render();

			for (Entity* childEntity: entity->children) {
				traverseAndUpdate(childEntity);
			}
		}
	}

	void SceneManager::traverseAndEmit(Entity* entity, Event& event) {
		if (entity) {
			// DFS traversal
			for (Entity* childEntity: entity->children) {
				traverseAndUpdate(childEntity);
			}

			entity->onEvent(event);
		}
	}

	// private:
	Scene* SceneManager::scene = nullptr;
}