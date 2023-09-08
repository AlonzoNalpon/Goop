#pragma once

#include "../System/System.h"
#include "../EntityComponentSystem.h"
#include "NumberComponent.h"
#include "TextComponent.h"
#include <iostream>
#include <sstream>

class PrintingSystem : public GE::ECS::System
{
public:
	void Awake()
	{
		std::cout << "Printing system awoken\n";
	}

	void Start()
	{
		std::cout << "Printing system started\n";
	}

	void Update()
	{
		std::cout << "I have entities with numbers of:\n";
		for (auto& entt : m_entities)
		{
			GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();

			Text* txt = ecs.GetComponent<Text>(entt);
			Number* num = ecs.GetComponent<Number>(entt);

			std::stringstream ss;
			ss << txt->text << num->a << ", " << num->b << ", " << num->c << ". Total: " << num->total << "\n";
			txt->text = ss.str();

			std::cout << txt->text;
		}
		std::cout << "Printing system updated\n";
	}

	void OnDestroyed()
	{
		std::cout << "Printing system destroyed\n";
	}
};
