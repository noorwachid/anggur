#pragma once

#include "Anggur/Core/Scene/Scene.h"

namespace Anggur {

	class SceneManager {
	public:
		static void initialize();
		static void terminate();

		static Scene* get();

		static void set(Scene* scene);
		static void update();
		static void emit(Event& event);

	private:
		static void traverseAndInitialize(Entity* entity);
		static void traverseAndTerminate(Entity* entity);

		static void traverseAndUpdate(Entity* entity);
		static void traverseAndEmit(Entity* entity, Event& event);

	private:
		static Scene* scene;
	};
}