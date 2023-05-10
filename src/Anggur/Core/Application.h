#pragma once

#include "Anggur/Core/Scene/Scene.h"
#include "Anggur/OS/Clock.h"
#include "Anggur/OS/Input.h"
#include "Anggur/OS/Window.h"
#include "Anggur/OS/WindowManager.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include <thread>

namespace Anggur
{
	class Application
	{
	public:
		WindowManager windowManager;
		Window* window;

		Renderer* renderer;
		Clock clock;

	public:
		Application();

		void Run(Scene* scene)
		{
			scene->window = window;
			scene->renderer = renderer;

			scene->Initialize();

			window->SetObserver(scene);

			clock.Tick();

			while (window->IsOpen())
			{
				windowManager.PollEvents();

				scene->Update(clock.Tick());
				scene->Draw();
				
				window->Update();

				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
		}
	};
}
