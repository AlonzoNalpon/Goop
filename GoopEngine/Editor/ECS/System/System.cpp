#include "System.h"

#ifdef _DEBUG
#include <iostream>
#endif

using namespace GE::ECS;

void System::Awake()
{
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

void GE::ECS::System::UpdateEntities()
{
	for (auto& entt : m_entities)
	{
		Update(const_cast<Entity&>(entt));
	}
}

void System::Update(Entity& /*entity*/)
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
