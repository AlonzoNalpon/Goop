#pragma once

#include "../System/System.h"
#include "../EntityComponentSystem.h"
#include "NumberComponent.h"
#include <iostream>

class AdditionSystem : public GE::ECS::System
{
public:
	void Awake()
	{
		std::cout << "Addition system awoken\n";
	}

	void Start()
	{
		std::cout << "Addition system started\n";
	}

	void Update()
	{
		for (auto& entt : m_entities)
		{
			Number* num = GE::ECS::EntityComponentSystem::GetInstance().GetComponent<Number>(entt);
			if (num == nullptr)
			{
				return;
			}
			num->total = num->a + num->b + num->c;
			std::cout << "Addition system updated\n";
		}
	}

	void OnDestroyed()
	{
		std::cout << "Addition system destroyed\n";
	}
};
