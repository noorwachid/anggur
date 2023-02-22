#pragma once

#include "Anggur/Event.h"
#include "Anggur/Scene/Entity.h"

namespace Anggur
{
	class Script : public Entity
	{
	public:
		virtual void Create()
		{
		}

		virtual void Update(float deltaTime)
		{
		}

		virtual void FixedUpdate(float deltaTime)
		{
		}

		virtual void On(Event& event)
		{
		}

		virtual void Destroy()
		{
		}

		template <typename T> void Bind()
		{
			instanciate = []() { return static_cast<Script*>(new T()); };
			shutdown = [](Script* instance) {
				delete static_cast<T*>(instance);
				instance = nullptr;
			};
		}

	private:
		Script* instance = nullptr;

		Script* (*instanciate)();

		void (*shutdown)(Script*);

	private:
		friend class Scene;
	};
}
