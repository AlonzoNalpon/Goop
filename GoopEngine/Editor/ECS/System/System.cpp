#include "System.h"
#include "../EntityComponentSystem.h"
#include <Component/Transform.h>

using namespace GE::ECS;

// Anonymous namespace for help function
namespace
{
	/*!*********************************************************************
	\brief 
	  Propergates upwards the entity parent tree until a parent is inactive
		or the parent is root and returns the active state

	\param[in] Entity
		Entity to start from

	\param[in] ecs
		ECS context

	return
		State of ancestor
	************************************************************************/
	bool IsAncestorActive(Entity& entity, EntityComponentSystem& ecs);
}

void System::Awake()
{
	m_ecs = &EntityComponentSystem::GetInstance();
}

void System::Start()
{
}

void System::Update()
{
}

void System::LateUpdate()
{
}

void System::FixedUpdate()
{
}

void System::OnDestroyed()
{
}

std::set<Entity>& GE::ECS::System::GetUpdatableEntities()
{
	m_updatables.clear();

	for (Entity entity : m_entities)
	{
		if (IsAncestorActive(entity, *m_ecs))
		{
			m_updatables.insert(entity);
		}
	}

	return m_updatables;
}

std::set<Entity>& System::GetEntities()
{
	return m_entities;
}

std::set<Entity>& System::GetInActiveEntities()
{
	return m_inactiveEntities;
}

std::set<Entity>& System::GetAllEntities()
{
	return m_allEntities;
}

void System::EntityActiveStateChanged(Entity& entity, bool newState)
{
	// Only update sets if entity exist in system
	if (m_entities.find(entity) != m_entities.end() ||
		m_inactiveEntities.find(entity) != m_inactiveEntities.end())
	{
		// Adding entity to active list
		if (newState)
		{
			m_entities.insert(entity);
			m_inactiveEntities.erase(entity);
		}
		// Adding entity to inactive list
		else
		{
			m_entities.erase(entity);
			m_inactiveEntities.insert(entity);
		}
	}
}

namespace
{
	bool IsAncestorActive(Entity& entity, EntityComponentSystem& ecs)
	{
		Entity parent = ecs.GetParentEntity(entity);
		if (parent != GE::ECS::INVALID_ID)
		{
			if (ecs.GetIsActiveEntity(parent))
			{
				return IsAncestorActive(entity, ecs);
			}

			// One of the parent is inactive, all children is considered inactive
			return false;
		}
		// End condition, parent has no parent
		else
		{
			return ecs.GetIsActiveEntity(entity);
		}
	}
}