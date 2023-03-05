#pragma once

#include "Anggur/Event.h"
#include "Anggur/IO/Input.h"
#include "Anggur/Math/Vector3.h"
#include "Anggur/Scene/Scene.h"
#include <entt/entt.hpp>
#include <memory>
#include <vector>

namespace Anggur
{
	class Scene;

	using EntityID = entt::entity;

	class Entity
	{
		friend class Scene;
		friend class Application;

	public:
		Entity() = default;

		Entity(const Entity& object) = default;

		Entity(EntityID id) : id{id}
		{
		}

		Entity Spawn()
		{
			Entity object(scene->registry.create());
			object.scene = scene;
			return object;
		}

	public:
		template <typename T, typename... Args> T& AddComponent(Args&&... args)
		{
			T& component = scene->registry.emplace<T>(id, std::forward<Args>(args)...);

			return component;
		}

		template <typename T, typename... Args> T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = scene->registry.emplace_or_replace<T>(id, std::forward<Args>(args)...);

			return component;
		}

		template <typename T> T& GetComponent()
		{
			return scene->registry.get<T>(id);
		}

		template <typename T> void RemoveComponent()
		{
			scene->registry.remove<T>(id);
		}

	protected:
		Window* window;

		Input* input;

	private:
		Scene* scene;
		EntityID id{entt::null};
	};
}
