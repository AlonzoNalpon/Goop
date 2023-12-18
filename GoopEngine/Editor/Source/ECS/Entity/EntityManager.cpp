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
#include <Events/EventManager.h>

using namespace GE::ECS;

EntityManager::EntityManager(unsigned int maxEntities) : 
	m_maxEntities{ maxEntities }
{
	m_mapOfActive.resize(maxEntities);
	m_entitySignatures.resize(maxEntities);
	// Push back full list of entities as available
	for (Entity i{0}; i < m_entitySignatures.size(); ++i)
	{
		m_availableEntities.insert(i);
	}
	m_parent.resize(maxEntities);
	std::fill(m_parent.begin(), m_parent.end(), INVALID_ID);
	m_children.resize(maxEntities);
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

	Entity entity = *m_availableEntities.begin();
	m_availableEntities.erase(entity);

	// Clear component bitset signature
	m_entitySignatures[entity].reset();
	m_mapOfActive[entity] = true;
	m_entities.insert(entity);
	m_names[entity] = "Entity " + std::to_string(entity);

	return entity;
}

void GE::ECS::EntityManager::CreateEntity(Entity entity, std::string name)
{
	if (m_entities.contains(entity))
	{
		throw GE::Debug::Exception<EntityManager>(GE::Debug::LEVEL_ERROR, ErrMsg("Creating an entity that already exist"));
	}

	m_availableEntities.erase(entity);
	// Clear component bitset signature
	m_entitySignatures[entity].reset();
	m_mapOfActive[entity] = true;
	m_entities.insert(entity);
	m_names[entity] = name;

}

void EntityManager::DestroyEntity(Entity& entity)
{
	GE::Debug::ErrorLogger::GetInstance().LogMessage<EntityManager>("Destroyed entity of ID " + std::to_string(entity), false);
	// Clear component bitset signature
	m_entitySignatures[entity].reset();
	m_mapOfActive[entity] = false;
	if (m_entities.erase(entity))
	{
		m_availableEntities.insert(entity);
	}
	m_names.erase(entity);

	if (m_parent[entity] != INVALID_ID)
	{
		RemoveChildEntity(m_parent[entity], entity);
	}
	m_parent[entity] = INVALID_ID;

	// Recursively destroy all children
	// Create a temp copy of m_children as you should not
	// delete while iterating
	std::set<Entity> originalList{ m_children[entity] };
	for (Entity childEntity : originalList)
	{
		DestroyEntity(childEntity);
	}
	m_children[entity].clear();
	Events::EventManager::GetInstance().Dispatch(Events::RemoveEntityEvent(entity));
}

bool GE::ECS::EntityManager::IsActiveEntity(Entity& entity)
{
	if (entity == INVALID_ID)
	{
		return false;
	}

	return m_mapOfActive[entity];
}

void GE::ECS::EntityManager::SetActiveEntity(Entity& entity, bool active)
{
	if (entity == INVALID_ID)
	{
		return;
	}

	m_mapOfActive[entity] = active;
}

Entity GE::ECS::EntityManager::GetParentEntity(Entity const& entity) const
{
	return m_parent[entity];
}

void GE::ECS::EntityManager::SetParentEntity(Entity const& parent, Entity const& child)
{
	if (child == INVALID_ID)
	{
		return;
	}

	m_parent[child] = parent;
}

std::set<Entity>& GE::ECS::EntityManager::GetChildEntities(Entity const& parent)
{
	if (parent == INVALID_ID)
	{
		throw GE::Debug::Exception<EntityComponentSystem>(GE::Debug::LEVEL_ERROR, ErrMsg("Unable to get child from non existent parent"));
	}

	return m_children[parent];
}

void GE::ECS::EntityManager::AddChildEntity(Entity const& parent, Entity const& child)
{
	m_children[parent].insert(child);
}

void GE::ECS::EntityManager::RemoveChildEntity(Entity& parent, Entity& child)
{
	m_children[parent].erase(child);
}

std::string GE::ECS::EntityManager::SetEntityName(Entity const& entity, std::string newName)
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

std::string GE::ECS::EntityManager::GetEntityName(Entity const& entity)
{
	// Entity should not exist
	if (m_names.find(entity) == m_names.end())
	{
		std::stringstream ss;
		ss << "Getting name of entitiy id " << entity << " that should not exist";
		throw GE::Debug::Exception<EntityManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg(ss.str()));
	}
	else
	{
		return m_names[entity];
	}
}

Entity GE::ECS::EntityManager::GetEntity(std::string const& name)
{
	for (auto const& entity : m_names)
	{
		if (entity.second == name)
		{
			return entity.first;
		}
	}
	return Entity();
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
