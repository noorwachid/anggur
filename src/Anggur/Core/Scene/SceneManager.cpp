#include "Anggur/Core/Scene/SceneManager.h"

namespace Anggur 
{
	void SceneManager::SetWindow(Window* newWindow)
	{
		window = newWindow;
	}

	void SceneManager::SetRenderer(Renderer* newRenderer)
	{
		renderer = newRenderer;
	}

	void SceneManager::SetScene(Scene* newScene)
	{
		if (scene)
		{
			Terminate();
		}

		scene = newScene;
		Initialize();
	}

	void SceneManager::Initialize()
	{
		window->SetListener(scene);

		if (scene)
		{
			scene->SetWindow(window);
			scene->SetRenderer(renderer);
			scene->Initialize();
		}
	}

	void SceneManager::Update(float deltaTime) 
	{
		if (scene)
			scene->Update(deltaTime);
	}

	void SceneManager::Draw()
	{
		if (scene)
			scene->Draw();
	}

	void SceneManager::Terminate()
	{
		if (scene)
		{
			delete scene;
			scene = nullptr;
		}
	}
}
