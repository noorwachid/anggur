#include "Anggur/Application.h"
#include "Anggur/Graphics/Function.h"
#include "Anggur/IO/Loop.h"
#include <thread>

namespace Anggur
{
	Application::Application()
	{
		window = new Window(Vector2(800, 600), "Anggur", [](void* procAddress)
		{
			return gladLoadGLLoader((GLADloadproc) procAddress);
		});

		renderer = new Renderer();
	}

	Application::~Application()
	{
		delete renderer;
		delete window;
	}

	void Application::Run(Scene* scene, const std::vector<std::string>& arguments)
	{
		ANGGUR_INSTRUMENTATION_SESSION_BEGIN("Anggur::Application::Run");

		process.arguments = arguments;

		this->scene = scene;

		scene->process = &process;
		scene->window = window;
		scene->renderer = renderer;
		scene->Initialize();

		window->SetObserver(scene);

		frameClock.Tick();

		while (window->IsOpen())
		{
			sleepClock.Tick();

			ANGGUR_INSTRUMENTATION_PROFILE("Anggur::Scene::Run::Loop");

			float deltaTime = frameClock.Tick();

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

			IO::Run(IO::RunMode::NoWait);

			float sleepTime = sleepClock.Tick();
			int sleepMiliseconds = Math::Max(1, 16 - int(sleepTime * 1000));

			std::this_thread::sleep_for(std::chrono::milliseconds(sleepMiliseconds));
		}

		ANGGUR_INSTRUMENTATION_SESSION_END;
	}
}
