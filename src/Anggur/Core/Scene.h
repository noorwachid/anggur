#pragma once

#include "Event/Event.h"

namespace Anggur
{
	class Scene
	{
	public:
		virtual void OnCreate()
		{
		}

		virtual void OnUpdate()
		{
		}

		virtual void OnDestroy()
		{
		}

		virtual void OnEvent(Event& event)
		{
		}

		virtual ~Scene()
		{
		}
	};
}
