#include "System.h"
#include "../EntityComponentSystem.h"

#ifdef _DEBUG
#include <iostream>
#endif

using namespace GE::ECS;

void System::Awake()
{
	m_ecs = &EntityComponentSystem::GetInstance();
#ifdef _DEBUG
	std::cout << __FUNCSIG__ << " Called\n";
#endif
}

void System::Start()
{
#ifdef _DEBUG
	std::cout << __FUNCSIG__ << " Called\n";
#endif
}

void System::Update()
{
#ifdef _DEBUG
	std::cout << __FUNCSIG__ << " Called\n";
#endif
}

void System::LateUpdate()
{
#ifdef _DEBUG
	std::cout << __FUNCSIG__ << " Called\n";
#endif
}

void System::FixedUpdate()
{
#ifdef _DEBUG
	std::cout << __FUNCSIG__ << " Called\n";
#endif
}

void System::OnDestroyed()
{
#ifdef _DEBUG
	std::cout << __FUNCSIG__ << " Called\n";
#endif
}

std::set<Entity>& System::GetEntities()
{
	return m_entities;
}
