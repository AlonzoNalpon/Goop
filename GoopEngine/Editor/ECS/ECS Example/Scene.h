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
	EntityComponentSystem* ecs;

	void Start()
	{
		ecs = &EntityComponentSystem::GetInstance();

		////////////////////////////////////////////////
		// Initializing existing system and components
		////////////////////////////////////////////////
		// The order of registration effects the order which the components are updated
		// use late update if your component relies on other systems to update them first
		// Flipping this 2 should give different results.
//#define OLD_SIGNATURE
#ifdef OLD_SIGNATURE
		ecs->RegisterSystem<AdditionSystem>();
		ecs->RegisterSystem<PrintingSystem>();

		ecs->RegisterComponent<Text>();
		ecs->RegisterComponent<Number>();

		ComponentSignature sig;
		// Set the signature of number component
		sig.set(ecs->GetComponentSignature<Number>(), true);

		// Tell ECS that addition system has the signature
		// that contains number component
		ecs->SetSystemSignature<AdditionSystem>(sig);

		// Add the signature of text component to first signature
		sig.set(ecs->GetComponentSignature<Text>(), true);
		// Tell ECS that printing system has the signature
		// that contains text & number component
		ecs->SetSystemSignature<PrintingSystem>(sig);
#else // NEW_SIGNATURE
		ecs->RegisterComponentToSystem<Number, AdditionSystem>();
		ecs->RegisterComponentToSystem<Number, PrintingSystem>();
		ecs->RegisterComponentToSystem<Text, PrintingSystem>();
#endif // OLD_SIGNATURE

		////////////////////////////////////////////////
		// Creating Entities w/ Components
		////////////////////////////////////////////////
		Number num1, num2;;
		num1.a = 0, num1.b = 2, num1.c = 4;
		num2.a = 0, num2.b = 2, num2.c = 4;
		Text txt; 
		txt.text = "My numbers are: ";
		Entity entt1 = ecs->CreateEntity();
		Entity entt2 = ecs->CreateEntity();

		ecs->AddComponent(entt1, num1);
		ecs->AddComponent(entt2, num2);
		ecs->AddComponent(entt2, txt);

		// Manually add entities u want to be updated by a system
		ecs->RegisterEntityToSystem<AdditionSystem>(entt1);
		ecs->RegisterEntityToSystem<AdditionSystem>(entt2);

		// Subsequently you can destroy an entity, this will remove 
		// the entity from all systems
		//ecs->DestroyEntity(entt1);

		ecs->RegisterEntityToSystem<PrintingSystem>(entt2);
		// You can remove a component and it will unregister
		// from all systems that require that component
		//ecs->RemoveComponent<Text>(entt2);

		// Example of unregistering from a system
		//ecs->UnregisterEntityFromSystem<PrintingSystem>(entt2);
	}

	void Update()
	{
		// NOTE: Entity 2 does not print!!!
		ecs->UpdateSystems();

		std::cout << "Entity 2 does not print. This is a manual check on Entity 2's numbers component\n";

		EntityComponentSystem& ecs = EntityComponentSystem::GetInstance();
		Number* num = ecs.GetComponent<Number>(1);
		std::cout << "Entity's numbers are: " << num->a << ", " << num->b << ", " << num->c << ". Total: " << num->total << "\n";
	}

	void Exit()
	{
		// Example of removing systems
		ecs->RemoveSystem<AdditionSystem>();
		ecs->RemoveSystem<PrintingSystem>();
	}
};