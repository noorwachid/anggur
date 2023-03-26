#pragma once 

#include "Anggur/Scene/Scene.h"

namespace Anggur
{
	class SceneSystem : public NotificationEventListener, WindowEventListener
	{
	public:
		void SetWindow(Window* newWindow)
		{
			window = newWindow;
		}

		void SetRenderer(Renderer* newRenderer)
		{
			renderer = newRenderer;
		}

		void SetScene(Scene* newScene)
		{
			if (scene)
			{
				Terminate();
			}

			scene = newScene;
			Initialize();
		}

		void Initialize()
		{
			if (scene)
			{
				scene->SetWindow(window);
				scene->SetRenderer(renderer);
				scene->Initialize();
			}
		}

		void Update(float deltaTime) 
		{
			if (scene)
				scene->Update(deltaTime);
		}

		void Draw()
		{
			if (scene)
				scene->Draw();
		}

		void Terminate()
		{
			if (scene)
			{
				delete scene;
				scene = nullptr;
			}
		}

	private:
		Window* window = nullptr;
		Renderer* renderer = nullptr;
		Scene* scene = nullptr; 
	};
}
