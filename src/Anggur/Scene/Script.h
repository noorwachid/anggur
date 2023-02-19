#pragma once 

#include "Anggur/Event.h"
#include "Entity.h"

namespace Anggur 
{
    class ScriptEntity : public Entity
    {
    public:
        virtual void Create() {}

        virtual void Update(float deltaTime) {}

        virtual void FixedUpdate(float deltaTime) {}

        virtual void On(Event& event) {}

        virtual void Destroy() {}
    };

    class Script
    {
    public:
        ScriptEntity* instance = nullptr;

		ScriptEntity* (*spawn)();
		void (*kill)(ScriptEntity*);

		template<typename T>
		void Bind()
		{
			spawn = []() { return static_cast<ScriptEntity*>(new T()); };
			kill = [](ScriptEntity* instance) { delete (T*) instance; instance = nullptr; };
		}
    };
}
