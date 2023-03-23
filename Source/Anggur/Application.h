#pragma once

#include "Anggur/IO/Clock.h"
#include "Anggur/IO/Input.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/WindowSystem.h"
#include "Anggur/Render/CanvasRenderer.h"
#include "Anggur/Render/UIRenderer.h"

namespace Anggur
{
	class Application
	{
	public:
		virtual void Initialize()
		{
			window.SetListener([this](Event& event) { On(event); });
		}

		virtual void Update()
		{
		}

		virtual void On(Event& event)
		{
		}

		virtual void Run()
		{
			Initialize();

			clock.Tick();

			while (window.IsOpen())
			{
				windowSystem.PollEvents();

				Update();

				window.Update();
			}
		}

	protected:
		WindowSystem windowSystem;
		Window window;

		CanvasRenderer canvasRenderer;
		UIRenderer uiRenderer;
		Clock clock;
	};
}
