#include "EntityComponentSystem.h"
#include "../AssetManager/AssetManager.h"

using namespace GE::ECS;

EntityComponentSystem::EntityComponentSystem()
{
	m_componentManager = new ComponentManager();
	m_entityManager = new EntityManager(GE::AssetManager::AssetManager::GetInstance().GetConfigData<unsigned>("Max Entities").value());
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

void GE::ECS::EntityComponentSystem::SetIsActiveEntity(Entity& entity, bool active)
{
	m_entityManager->SetActiveEntity(entity, active);
	m_systemManager->EntityActiveStateChanged(entity, active);
}

void EntityComponentSystem::DestroyEntity(Entity& entity)
{
	m_entityManager->DestroyEntity(entity);
	m_componentManager->EntityDestroyed(entity);
	m_systemManager->EntityDestroyed(entity);
}

std::set<Entity>& GE::ECS::EntityComponentSystem::GetEntities()
{
	return m_entityManager->GetEntities();
}

ComponentSignature GE::ECS::EntityComponentSystem::GetComponentSignature(Entity& entity)
{
	return m_entityManager->GetComponentSignature(entity);
}

void EntityComponentSystem::UpdateSystems()
{
	m_systemManager->UpdateSystems();
}


