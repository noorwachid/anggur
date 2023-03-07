#pragma once

#include "Anggur/IO/Clock.h"
#include "Anggur/IO/Input.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/WindowSystem.h"
#include "Anggur/Render/MeshRenderer.h"
#include "Anggur/Scene/Scene.h"
#include "Anggur/Scene/SceneSystem.h"
#include "Anggur/Scene/Script.h"
#include "Anggur/Scene/Transform.h"

namespace Anggur
{
	class Application
	{
	public:
		virtual void Initialize()
		{
			window.SetListener([this](Event& event) { sceneSystem.On(event); });

			viewport = window.GetSize();

			sceneSystem.window = &window;
			sceneSystem.meshRenderer = &meshRenderer;
			sceneSystem.viewport = &viewport;
		}

		virtual void Run(Scene* newScene)
		{
			Initialize();

			sceneSystem.Attach(newScene);

			clock.Tick();

			while (window.IsOpen())
			{
				windowSystem.PollEvents();

				sceneSystem.Update(clock.Tick());

				window.Update();
			}

			sceneSystem.Detach();
		}

	protected:
		WindowSystem windowSystem;
		Window window;

		Vector2 viewport;

		MeshRenderer meshRenderer;
		Clock clock;

		SceneSystem sceneSystem;
	};
}
