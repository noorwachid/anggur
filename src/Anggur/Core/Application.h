#pragma once

#include "Anggur/OS/Clock.h"
#include "Anggur/OS/Input.h"
#include "Anggur/OS/Window.h"
#include "Anggur/OS/WindowManager.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/Core/Scene/SceneManager.h"
#include <thread>

namespace Anggur
{
	class Application
	{
	public:
		WindowManager windowSystem;
		Window* window;

		Renderer* renderer;
		Clock clock;

		SceneManager sceneSystem;

	public:
		Application();

		void Run()
		{
			clock.Tick();

			while (window->IsOpen())
			{
				windowSystem.PollEvents();

				sceneSystem.Update(clock.Tick());
				sceneSystem.Draw();
				
				window->Update();

				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
		}
	};
}
