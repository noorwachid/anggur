#pragma once 

#include "Anggur/Event.h"
#include "Anggur/IO/Input.h"
#include "Anggur/Math/Vector3.h"
#include "Registry.h"
#include <cassert>
#include <memory>
#include <vector>

namespace Anggur
{
    using EntityID = entt::entity;

    class Entity
    {
        friend class Registry;
        friend class Application;

    public:
        virtual void Initialize() {}

        virtual void Update(float deltaTime) {}

        virtual void FixedUpdate(float deltaTime) {}

        virtual void On(Event& event) {}

        virtual void Terminate() {}

    public:
        template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = registry->database.emplace<T>(id, std::forward<Args>(args)...);

			return component;
		}

        template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = registry->database.emplace_or_replace<T>(id, std::forward<Args>(args)...);

			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			return registry->database.get<T>(id);
		}

		template<typename T>
		void RemoveComponent()
		{
			registry->database.remove<T>(id);
		}

    protected:
        Window* window;

    private:
        Registry* registry;
        EntityID id{entt::null};
    };
}
