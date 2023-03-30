#pragma once

#include <vector>

namespace Anggur
{
	class Window;
	class Renderer;
	class Scene;

	class Node 
	{
	public:
		virtual void Initialize() {}

		virtual void Update(float deltaTime) {}

		virtual void Draw() {}

		virtual void LateDraw() {}

		template <typename T>
		T* Spawn()
		{
			T* node = new T;
			node->window = window;
			node->renderer = renderer;
			node->scene = scene;

			node->Initialize();

			children.push_back(node);

			return node;
		}

	public:

		Window* window = nullptr;
		Renderer* renderer = nullptr;
		Scene* scene = nullptr;

		Node* parent = nullptr;
		std::vector<Node*> children;

		friend class Scene;
	};
}
