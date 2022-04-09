#pragma once

#include "Anggur/Core/Window.h"
#include "Anggur/Core/Scene/Scene.h"

namespace Anggur {
	class Application {
	public:
		static Window* getWindow();

		static void run(Scene* scene);

		static void setVsync(bool enable = true);

		static void initialize();
		static void terminate();

	private:
		Application();

	private:
		static Window* window;
	};
}
