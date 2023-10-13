#include "System.h"
#include "../EntityComponentSystem.h"

#ifdef _DEBUG
#include <iostream>
#endif

using namespace GE::ECS;

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
