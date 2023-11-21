/*!*********************************************************************
\file   EntityComponentSystem.cpp
\author w.chinkitbryan\@digipen.edu
\date   9-September-2023
\brief  
  Main coordinator and interface for all the Entity Component System's 
	sub system managers.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "EntityComponentSystem.h"
#include "../AssetManager/AssetManager.h"

using namespace GE::ECS;

void GE::ECS::EntityComponentSystem::CreateEntity(Exclusive, Entity entity, std::string name)
{
	m_entityManager->CreateEntity(entity, name);
}

EntityComponentSystem::EntityComponentSystem()
{
	m_componentManager = std::make_unique<ComponentManager>();
	std::optional<unsigned> maxEntitiesCnt = GE::Assets::AssetManager::GetInstance().GetConfigData<unsigned>("Max Entities");
	// 4092 default if value not found
	m_entityManager = std::make_unique<EntityManager>(maxEntitiesCnt ? maxEntitiesCnt.value() : 4092u);
	m_systemManager = std::make_unique<SystemManager>();
}

unsigned int GE::ECS::EntityComponentSystem::GetMaxEntities()
{
	return m_entityManager->m_maxEntities;
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

Entity GE::ECS::EntityComponentSystem::GetParentEntity(Entity& entity)
{
	return m_entityManager->GetParentEntity(entity);
}

void GE::ECS::EntityComponentSystem::SetParentEntity(Entity const& child, Entity parent)
{
	m_entityManager->SetParentEntity(parent, child);
}

std::set<Entity>& GE::ECS::EntityComponentSystem::GetChildEntities(Entity& parent)
{
	return m_entityManager->GetChildEntities(parent);
}

void GE::ECS::EntityComponentSystem::AddChildEntity(Entity const& parent, Entity const& child)
{
	m_entityManager->AddChildEntity(parent, child);
}

void GE::ECS::EntityComponentSystem::RemoveChildEntity(Entity& parent, Entity& child)
{
	m_entityManager->RemoveChildEntity(parent, child);
}

void EntityComponentSystem::DestroyEntity(Entity& entity)
{
	m_entityManager->DestroyEntity(entity);
	m_componentManager->EntityDestroyed(entity);
	m_systemManager->EntityDestroyed(entity);
}

std::string GE::ECS::EntityComponentSystem::GetEntityName(Entity& entity)
{
	return m_entityManager->GetEntityName(entity);
}

std::string GE::ECS::EntityComponentSystem::SetEntityName(Entity& entity, std::string newName)
{
	return m_entityManager->SetEntityName(entity, newName);
}

std::set<Entity>& GE::ECS::EntityComponentSystem::GetEntities()
{
	return m_entityManager->GetEntities();
}

ComponentSignature GE::ECS::EntityComponentSystem::GetComponentSignature(Entity& entity)
{
	return m_entityManager->GetComponentSignature(entity);
}

std::unordered_map<const char*, ComponentSignature>& GE::ECS::EntityComponentSystem::GetSystemSignatures()
{
	return m_systemManager->GetSystemSignatures();
}

std::map<int, const char*>& GE::ECS::EntityComponentSystem::GetSystemIndexes()
{
	return m_systemManager->GetSystemIndexes();
}

void EntityComponentSystem::UpdateSystems()
{
	m_systemManager->UpdateSystems();
}

#ifndef NO_IMGUI
void GE::ECS::EntityComponentSystem::UpdateSystems(int systemCount, ...)
{
	va_list args;
	va_start(args, systemCount);

	m_systemManager->UpdateSystems(systemCount, args);
	va_end(args);
}
#endif // !NO_IMGUI
