#pragma once

#include "Anggur/IO/Clock.h"
#include "Anggur/IO/Input.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/WindowSystem.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/Scene/SceneSystem.h"
#include <thread>

namespace Anggur
{
	class Application
	{
	public:
		WindowSystem windowSystem;
		Window window;

		Renderer renderer;
		Clock clock;

		SceneSystem sceneSystem;

	public:
		Application()
		{
			sceneSystem.SetWindow(&window);
			sceneSystem.SetRenderer(&renderer);

		}

		void Run()
		{
			clock.Tick();

			while (window.IsOpen())
			{
				windowSystem.PollEvents();

				sceneSystem.Update(clock.Tick());
				sceneSystem.Draw();

				window.Update();

				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
		}
	};
}
