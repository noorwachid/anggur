#pragma once

#include "Anggur/Event.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/WindowEvent.h"
#include "Anggur/Scene/Node.h"
#include <vector>

namespace Anggur
{
	class Scene : public NotificationEventListener, public WindowEventListener
	{
	public:
		virtual void Initialize();

		virtual void Update(float deltaTime);

		virtual void Draw();

		template <typename T>
		T* Spawn()
		{
			T* node = new T;
			node->window = window;
			node->renderer = renderer;
			node->scene = this;

			node->Initialize();

			nodes.push_back(node);

			return node;
		}

		void SetWindow(Window* newWindow);

		void SetRenderer(Renderer* newRenderer);

	private:
		void RecursiveUpdate(Node* node, float deltaTime);

		void RecursiveDraw(Node* node);


	protected:
		Window* window = nullptr;
		Renderer* renderer = nullptr;

		std::vector<Node*> nodes;
	};
}
