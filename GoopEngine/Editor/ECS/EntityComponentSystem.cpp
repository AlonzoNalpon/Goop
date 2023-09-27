#include "EntityComponentSystem.h"

using namespace GE::ECS;

EntityComponentSystem::EntityComponentSystem()
{
	m_componentManager = new ComponentManager();
	m_entityManager = new EntityManager(4092);
	m_systemManager = new SystemManager();
}

EntityComponentSystem::~EntityComponentSystem()
{
	delete m_componentManager;
	m_componentManager = nullptr;

	delete m_entityManager;
	m_entityManager = nullptr;

	delete m_systemManager;
	m_systemManager = nullptr;
}

Entity EntityComponentSystem::CreateEntity()
{
	return m_entityManager->CreateEntity();
}

bool GE::ECS::EntityComponentSystem::GetIsActiveEntity(Entity& entity)
{
	return m_entityManager->IsActiveEntity(entity);
}

void EntityComponentSystem::DestroyEntity(Entity& entity)
{
	m_entityManager->DestroyEntity(entity);
	m_componentManager->EntityDestroyed(entity);
	m_systemManager->EntityDestroyed(entity);
}

void EntityComponentSystem::UpdateSystems()
{
	m_systemManager->UpdateSystems();
}
