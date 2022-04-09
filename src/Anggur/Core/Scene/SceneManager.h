#pragma once

#include "Anggur/Core/Scene/Scene.h"

namespace Anggur {

	class SceneManager {
	public:
		static Scene* get();

		static void set(Scene* scene);

		static void update();

		static void destroy();

	private:
		static Scene* data;
	};
}