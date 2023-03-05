#include "Anggur/Scene/SceneSystem.h"

namespace Anggur
{
	void SceneSystem::Attach()
	{
		if (scene)
		{
			scene->window = window;
			scene->meshRenderer = meshRenderer;
			scene->viewport = viewport;
			scene->Attach();
		}
	}

	void SceneSystem::Attach(Scene* newScene)
	{
		if (scene)
			scene->Detach();

		scene = newScene;

		Attach();
	}

	void SceneSystem::Update(float deltaTime)
	{
		if (scene)
			scene->Update(deltaTime);
	}

	void SceneSystem::On(Event& event)
	{
		if (scene)
			scene->On(event);
	}

	void SceneSystem::Detach()
	{
		if (scene)
			scene->Detach();
	}
}
