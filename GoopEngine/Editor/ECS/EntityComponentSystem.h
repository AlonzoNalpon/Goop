#pragma once
#include "Component/ComponentManager.h"
#include "Entity/EntityManager.h"
#include "System/SystemManager.h"

// Interface for using the different ECS systems
namespace GE
{
	namespace ECS
	{
		class EntityComponentSystem
		{
		public:
			EntityComponentSystem() = default;
			~EntityComponentSystem();

			void Init();
			Entity& CreateEntity();
			void DestroyEntity(Entity& entity);

			template <typename T>
			void AddComponent(Entity& entity, T component);
			template <typename T>
			void RemoveComponent(Entity& entity);
			template <typename T>
			T& GetComponent(const Entity& entity);
			template <typename T>
			ComponentType GetComponentType();
			template <typename T>
			T* RegisterSystem();
			template <typename T>
			void SetSystemSignature(const ComponentSignature& signature);

			void UpdateSystems();

		private:
			ComponentManager* m_componentManager;
			EntityManager* m_entityManager;
			SystemManager* m_systemManager;
		};

#include "EntityComponentSystem.tpp"
	}
}
