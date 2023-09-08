#pragma once

#include "Component/ComponentManager.h"
#include "Entity/EntityManager.h"
#include "System/SystemManager.h"
#include "../Singleton/Singleton.h"

// Interface for using the different ECS systems
namespace GE
{
	namespace ECS
	{
		class EntityComponentSystem : public Singleton<EntityComponentSystem>
		{
		public:
			EntityComponentSystem();
			~EntityComponentSystem();

			Entity CreateEntity();
			void DestroyEntity(Entity& entity);

			template <typename T>
			void RegisterComponent();
			template <typename T>
			void AddComponent(Entity& entity, T component);
			template <typename T>
			void RemoveComponent(Entity& entity);
			template <typename T>
			T* GetComponent(const Entity& entity);
			template <typename T>
			ComponentType GetComponentSignature();
			template <typename T>
			T* RegisterSystem();
			template <typename T>
			bool RemoveSystem();
			template <typename T>
			bool SetSystemSignature(const ComponentSignature& signature);
			template <typename T>
			bool RegisterEntityToSystem(Entity& entity);
			template <typename T>
			void UnregisterEntityFromSystem(Entity& entity);

			void UpdateSystems();

		private:
			ComponentManager* m_componentManager;
			EntityManager* m_entityManager;
			SystemManager* m_systemManager;
		};

#include "EntityComponentSystem.tpp"
	}
}
