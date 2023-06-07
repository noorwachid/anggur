#pragma once

#include "Anggur/Core/Instrumentation.h"
#include "Anggur/Core/Process.h"
#include "Anggur/OS/Clock.h"
#include "Anggur/OS/Input.h"
#include "Anggur/OS/Window.h"
#include "Anggur/OS/WindowManager.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/Studio/Scene.h"
#include <thread>

namespace Anggur
{
	class Application
	{
	public:
		WindowManager windowManager;
		Window* window;
		Renderer* renderer;
		Scene* scene;
		Clock clock;

		Process process;

	public:
		Application();

		void Run(Scene* scene, const std::vector<std::string>& arguments)
		{
			ANGGUR_INSTRUMENTATION_SESSION_BEGIN("Anggur::Application::Run");

			process.arguments = arguments;

			this->scene = scene;

			scene->process = &process;
			scene->window = window;
			scene->renderer = renderer;
			scene->Initialize();

			window->SetObserver(scene);

			clock.Tick();

			while (window->IsOpen())
			{
				ANGGUR_INSTRUMENTATION_PROFILE("Anggur::Scene::Run::Loop");

				float deltaTime = clock.Tick();

				{
					ANGGUR_INSTRUMENTATION_PROFILE("Anggur::Scene::Update");
					scene->Update(deltaTime);
				}

				{
					ANGGUR_INSTRUMENTATION_PROFILE("Anggur::Scene::Draw");
					scene->Draw();
				}
				
				window->Update();

				windowManager.PollEvents();

				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}

			ANGGUR_INSTRUMENTATION_SESSION_END;
		}
	};
}
