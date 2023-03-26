#include "Anggur/Scene/Scene.h"

namespace Anggur
{
	void Scene::Initialize() {}

	void Scene::Update(float deltaTime) {}

	void Scene::Draw() {}

	void Scene::SetWindow(Window* newWindow)
	{
		window = newWindow;
	}

	void Scene::SetRenderer(Renderer* newRenderer) 
	{
		renderer = newRenderer;
	}
}

