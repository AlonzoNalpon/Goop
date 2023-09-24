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
