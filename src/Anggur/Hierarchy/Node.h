#pragma once

#include <Anggur/Core/Event/Common.h>
#include <Anggur/System/Array.h>

namespace Anggur 
{
	class Node 
	{
	public:
		virtual void Initialize();
		virtual void Terminate();
		virtual void Update();
		virtual void Render();
		virtual void OnEvent(Event& event);

	public:
		Vector2 position;
		Vector2 scale;
		float rotation;

		Array<Node*> children;
	};
}