#pragma once

#include "Anggur/Core/Window.h"
#include "Anggur/Core/Scene/Scene.h"

namespace Anggur {
	class Application {
	public:
		static void initialize(const Vector2& windowSize, const std::string& windowTitle);
		static void terminate();

		static void run(Scene* scene, const Vector2& windowSize = Vector2(800, 600), const std::string& windowTitle = "");

		static void setVsync(bool enable = true);

		static Window* getWindow();

	private:
		Application();

	private:
		static Window* window;
	};
}
