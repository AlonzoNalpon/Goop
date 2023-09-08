#pragma once

#include "../EntityComponentSystem.h"
#include "AdditionSystem.h"
#include "PrintingSystem.h"
#include "TextComponent.h"
#include "NumberComponent.h"
#include <vector>

using namespace GE::ECS;

struct Scene
{
	void Start()
	{
		////////////////////////////////////////////////
		// Initializing existing system and components
		////////////////////////////////////////////////
		ecs->RegisterSystem<PrintingSystem>();
		ecs->RegisterSystem<AdditionSystem>();

		ecs->RegisterComponent<Text>();
		ecs->RegisterComponent<Number>();

		// Get the signature of number component
		ComponentSignature sig = ecs->GetComponentSignature<Number>();
		// Tell ECS that addition system has the signature
		// that contains number component
		ecs->SetSystemSignature<AdditionSystem>(sig);

		// Add the signature of text component to first signature
		sig.set(ecs->GetComponentSignature<Text>(), true);
		// Tell ECS that printing system has the signature
		// that contains text & number component
		ecs->SetSystemSignature<PrintingSystem>(sig);

		////////////////////////////////////////////////
		// Creating Entities w/ Components
		////////////////////////////////////////////////
		Number num1{0, 2, 4};
		Number num2{1, 3, 5};
		Text txt{ "My numbers are: " };
		Entity entt1 = ecs->CreateEntity();
		Entity entt2 = ecs->CreateEntity();

		ecs->AddComponent(entt1, num1);

		ecs->AddComponent(entt2, num2);
		ecs->AddComponent(entt2, txt);
		// Example of removing component
		//ecs->RemoveComponent<Text>(entt2);
	}

	void Update()
	{
		ecs->UpdateSystems();
	}

	void Exit()
	{
		// Example of removing systems
		ecs->RemoveSystem<AdditionSystem>();
		ecs->RemoveSystem<PrintingSystem>();
	}
};