#include "EntityManager.h"

using namespace GE::ECS;

EntityManager::EntityManager(unsigned int maxEntities) : 
	m_maxEntities{ maxEntities }, m_entitiesAlive{0}
{
	m_entities.resize(maxEntities);
	// Push back full list of entities as available
	for (Entity i{0}; i < m_entities.size(); ++i)
	{
		m_availableEntities.push(i);
	}
}

EntityManager::~EntityManager()
{
}

Entity EntityManager::CreateEntity()
{
	Entity entity = m_availableEntities.front();
	m_availableEntities.pop();

	m_entitiesAlive++;
	// Clear component bitset signature
	m_entities[entity].reset();
	return entity;
}

void EntityManager::DestroyEntity(Entity& entity)
{
	// Clear component bitset signature
	m_entities[entity].reset();
	m_availableEntities.push(entity);
	m_entitiesAlive--;
}

ComponentSignature EntityManager::GetComponentSignature(const Entity& entity) const
{
	return m_entities[entity];
}

void EntityManager::SetComponentSignature(Entity& entity, ComponentSignature& signature)
{
	m_entities[entity] = signature;
}
