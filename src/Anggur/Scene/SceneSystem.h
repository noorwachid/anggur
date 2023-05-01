#pragma once 

#include "Anggur/Scene/Scene.h"

namespace Anggur
{
	class SceneSystem
	{
	public:
		void SetWindow(Window* newWindow);

		void SetRenderer(Renderer* newRenderer);

		void SetScene(Scene* newScene);

		void Initialize();

		void Update(float deltaTime);

		void Draw();

		void Terminate();

	private:
		Window* window = nullptr;
		Renderer* renderer = nullptr;
		Scene* scene = nullptr; 
	};
}