#pragma once

#include "Anggur/Graphics/Render/Renderer.h"

namespace Anggur
{
	class Object
	{
	public:
		virtual void Initialize() = 0;

		virtual void Update(float deltaTime) = 0;

		virtual void Draw() = 0;
	
	protected:
		Renderer* renderer;

		std::vector<Object*> children;
	};
}
