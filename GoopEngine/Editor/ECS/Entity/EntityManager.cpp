/*!*********************************************************************
\file   EntityManager.cpp
\author w.chinkitbryan\@digipen.edu
\date   9-September-2023
\brief  
  Definitions of EntityManager functions declared in EntityManager.h
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include "EntityManager.h"
#include <DebugTools/ErrorLogger/ErrorLogger.h>
#include <DebugTools/Exception/Exception.h>
#include <limits>

using namespace GE::ECS;

EntityManager::EntityManager(unsigned int maxEntities) : 
	m_maxEntities{ maxEntities }, m_entitiesAlive{0}
{
	m_mapOfActive.resize(maxEntities);
	m_entitySignatures.resize(maxEntities);
	// Push back full list of entities as available
	for (Entity i{0}; i < m_entitySignatures.size(); ++i)
	{
		m_availableEntities.push(i);
	}
}

EntityManager::~EntityManager()
{
}

Entity EntityManager::CreateEntity()
{
	// Cap hit
	if (m_availableEntities.size() == 0)
	{
		throw GE::Debug::Exception<EntityManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Creating more entities than allowed"));
	}

	Entity entity = m_availableEntities.front();
	m_availableEntities.pop();

	m_entitiesAlive++;
	// Clear component bitset signature
	m_entitySignatures[entity].reset();
	m_mapOfActive[entity] = true;
	m_entities.insert(entity);
	m_names[entity] = "Entity " + std::to_string(entity);
	return entity;
}

void EntityManager::DestroyEntity(Entity& entity)
{
	GE::Debug::ErrorLogger::GetInstance().LogMessage<EntityManager>("Destroyed entity ID: " + entity, false);
	// Clear component bitset signature
	m_entitySignatures[entity].reset();
	m_mapOfActive[entity] = false;
	m_availableEntities.push(entity);
	m_entities.erase(entity);
	m_entitiesAlive--;
	m_names.erase(entity);
}

bool GE::ECS::EntityManager::IsActiveEntity(Entity& entity)
{
	return m_mapOfActive[entity];
}

void GE::ECS::EntityManager::SetActiveEntity(Entity& entity, bool active)
{
	m_mapOfActive[entity] = active;
}

std::string GE::ECS::EntityManager::SetEntityName(Entity& entity, std::string newName)
{
	// Entity should not exist
	if (m_names.find(entity) == m_names.end())
	{
		throw GE::Debug::Exception<EntityManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Setting name of entity that should not exist"));
	}
	else
	{
		return m_names[entity] = newName;
	}
}

std::string GE::ECS::EntityManager::GetEntityName(Entity& entity)
{
	// Entity should not exist
	if (m_names.find(entity) == m_names.end())
	{
		throw GE::Debug::Exception<EntityManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Getting name of entity that should not exist"));
	}
	else
	{
		return m_names[entity];
	}
}

std::set<Entity>& GE::ECS::EntityManager::GetEntities()
{
	return m_entities;
}

ComponentSignature EntityManager::GetComponentSignature(const Entity& entity) const
{
	return m_entitySignatures[entity];
}

void EntityManager::SetComponentSignature(Entity& entity, const ComponentSignature& signature)
{
	m_entitySignatures[entity] = signature;
}
