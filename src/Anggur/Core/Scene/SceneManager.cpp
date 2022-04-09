#include "Anggur/Core/Scene/SceneManager.h"

namespace Anggur {
	// public:
	Scene* SceneManager::get() {
		return data;
	}

	void SceneManager::set(Scene* scene) {
		destroy();

		data = scene;
		data->onAttach();
	}

	void SceneManager::update() {
		if (data) {
			data->onUpdate();
		}
	}

	void SceneManager::destroy() {
		if (data) {
			data->onDetach();
			delete data;
			data = nullptr;
		}
	}

	// private:
	Scene* SceneManager::data = nullptr;
}