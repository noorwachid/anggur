#include "Anggur/Scene/SceneSystem.h"

namespace Anggur 
{
	void SceneSystem::SetWindow(Window* newWindow)
	{
		window = newWindow;
	}

	void SceneSystem::SetRenderer(Renderer* newRenderer)
	{
		renderer = newRenderer;
	}

	void SceneSystem::SetScene(Scene* newScene)
	{
		if (scene)
		{
			Terminate();
		}

		scene = newScene;
		Initialize();
	}

	void SceneSystem::Initialize()
	{
		window->SetListener(scene);

		if (scene)
		{
			scene->SetWindow(window);
			scene->SetRenderer(renderer);
			scene->Initialize();
		}
	}

	void SceneSystem::Update(float deltaTime) 
	{
		if (scene)
			scene->Update(deltaTime);
	}

	void SceneSystem::Draw()
	{
		if (scene)
			scene->Draw();
	}

	void SceneSystem::Terminate()
	{
		if (scene)
		{
			delete scene;
			scene = nullptr;
		}
	}
}
