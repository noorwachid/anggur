#include "Anggur/Application.h"
#include "Anggur/Graphics/Function.h"
#include "Anggur/IO/Loop.h"
#include <thread>

namespace Anggur {
	Application::Application() {
		window = new Window(Vector2(800, 600), "Anggur", [](void* procAddress) {
			return gladLoadGLLoader((GLADloadproc)procAddress);
		});

		renderer = new Renderer();
	}

	Application::~Application() {
		delete renderer;
		delete window;
	}

	void Application::run(Scene* scene, const std::vector<std::string>& arguments) {
		ANGGUR_INSTRUMENTATION_SESSION_BEGIN("Anggur::Application::Run");

		process.arguments = arguments;

		this->scene = scene;

		scene->process = &process;
		scene->window = window;
		scene->renderer = renderer;
		scene->initialize();

		window->setObserver(scene);

		frameClock.getDeltaTime();

		while (window->isOpen()) {
			sleepClock.getDeltaTime();

			ANGGUR_INSTRUMENTATION_PROFILE("Anggur::Scene::Run::Loop");

			float deltaTime = frameClock.getDeltaTime();

			{
				ANGGUR_INSTRUMENTATION_PROFILE("Anggur::Scene::Update");
				scene->update(deltaTime);
			}

			{
				ANGGUR_INSTRUMENTATION_PROFILE("Anggur::Scene::Draw");
				scene->draw();
			}

			window->update();

			windowManager.pollEvents();

			Anggur::run(RunMode::noWait);

			float sleepTime = sleepClock.getDeltaTime();
			int sleepMiliseconds = Math::max(1, 16 - int(sleepTime * 1000));

			std::this_thread::sleep_for(std::chrono::milliseconds(sleepMiliseconds));
		}

		Anggur::run(RunMode::wait);

		ANGGUR_INSTRUMENTATION_SESSION_END;
	}
}
