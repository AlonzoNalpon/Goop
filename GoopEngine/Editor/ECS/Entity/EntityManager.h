#pragma once
#include "Entity.h"
#include <vector>
#include <queue>

namespace GE
{
	namespace ECS
	{
		class EntityManager
		{
		public:
			// Max entities in the game
			// Exposed read only
			const unsigned int m_maxEntities;
		private:
			// Current number of activiely used EntityIDs
			unsigned int m_entitiesAlive;

			std::vector<ComponentSignature> m_entities;
			std::queue<Entity> m_availableEntities;
		public:
			EntityManager() = delete;
			EntityManager(unsigned int maxEntities);
			~EntityManager();

			Entity CreateEntity();
			void DestroyEntity(Entity& entity);
			ComponentSignature GetComponentSignature(const Entity& entity) const;
			void SetComponentSignature(Entity& entity, ComponentSignature& signature);
		};
	}
}
