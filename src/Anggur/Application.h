#pragma once

#include "Anggur/Core/Instrumentation.h"
#include "Anggur/Core/Process.h"
#include "Anggur/OS/Clock.h"
#include "Anggur/OS/Input.h"
#include "Anggur/OS/Window.h"
#include "Anggur/OS/WindowManager.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/Studio/Scene.h"

namespace Anggur
{
	class Application
	{
	public:
		WindowManager windowManager;
		Window* window;
		Renderer* renderer;
		Scene* scene;

		Clock frameClock;
		Clock sleepClock;

		Process process;

	public:
		Application();

		~Application();

		void Run(Scene* scene, const std::vector<std::string>& arguments);
	};
}
