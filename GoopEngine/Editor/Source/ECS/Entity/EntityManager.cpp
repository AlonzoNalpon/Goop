/*!*********************************************************************
\file   EntityManager.cpp
\author w.chinkitbryan\@digipen.edu
\date   9-September-2023
\brief  
  Contains the info of the actual entities. And acts as an object pool.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "EntityManager.h"
#include <DebugTools/ErrorLogger/ErrorLogger.h>
#include <DebugTools/Exception/Exception.h>
#include <limits>
#include "Entity.h"

using namespace GE::ECS;

EntityManager::EntityManager(unsigned int maxEntities) : 
	m_maxEntities{ maxEntities }, m_entitiesAlive{0}
{
	m_entitySignatures.resize(maxEntities);
	// Push back full list of entities as available
	for (unsigned int i{}; i < m_maxEntities; ++i)
	{
		m_entities.emplace_back(i, "");
	}
}

EntityManager::~EntityManager()
{
}

Entity& EntityManager::CreateEntity()
{
	// Cap hit
	if (m_entitiesAlive >= m_maxEntities)
	{
		throw GE::Debug::Exception<EntityManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Creating more entities than allowed"));
	}

	for (Entity& entity : m_entities)
	{
		if (entity.m_active)
		{
			continue;
		}

		m_entitiesAlive++;
		m_entitySignatures[entity.m_id].reset();
		entity.m_active = true;

		return entity;
	}

	throw GE::Debug::Exception<EntityManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Creating more entities than allowed"));
}

void EntityManager::DestroyEntity(Entity& entity)
{
	GE::Debug::ErrorLogger::GetInstance().LogMessage<EntityManager>("Destroyed entity of ID " + entity.m_name, false);
	// Clear component bitset signature
	m_entitySignatures[entity.m_id].reset();
	m_entities[entity.m_id].m_active = false;

	if (entity.GetParent().m_id != INVALID_ID)
	{
		entity.GetParent().RemoveChildren(entity);
	}
	entity.SetParent();

	// Recursively destroy all children
	// Create a temp copy of m_children as you should not
	// delete while iterating
	std::set<Entity> originalList{ entity.GetChildren() };
	for (Entity childEntity : originalList)
	{
		DestroyEntity(childEntity);
	}
	entity.GetChildren().clear();
}

std::vector<Entity>& GE::ECS::EntityManager::GetEntities()
{
	return m_entities;
}

ComponentSignature EntityManager::GetComponentSignature(const Entity& entity) const
{
	return m_entitySignatures[entity.m_id];
}

void EntityManager::SetComponentSignature(Entity& entity, const ComponentSignature& signature)
{
	m_entitySignatures[entity.m_id] = signature;
}
