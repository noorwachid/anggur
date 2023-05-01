#include "Anggur/Core/Scene/Scene.h"

namespace Anggur
{
	void Scene::Initialize() {}

	void Scene::Update(float deltaTime) 
	{
		for (Node* node : nodes)
			RecursiveUpdate(node, deltaTime);
	}

	void Scene::Draw() 
	{
		for (Node* node : nodes)
			RecursiveDraw(node);
	}

	void Scene::SetWindow(Window* newWindow)
	{
		window = newWindow;
	}

	void Scene::SetRenderer(Renderer* newRenderer) 
	{
		renderer = newRenderer;
	}

	void Scene::RecursiveUpdate(Node* node, float deltaTime)
	{
		node->Update(deltaTime);

		for (Node* child : node->children)
			RecursiveUpdate(child, deltaTime);
	}

	void Scene::RecursiveDraw(Node* node)
	{
		node->Draw();

		for (Node* child : node->children)
			RecursiveDraw(child);
		
		node->LateDraw();
	}
}

