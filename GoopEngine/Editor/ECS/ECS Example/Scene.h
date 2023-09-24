#pragma once
#include <pch.h>

#include <Physics/PhysicsSystem.h>
#include <Physics/CollisionSystem.h>
#include <DraggableObject/DraggableObjectSystem.h>

#include <Component/Velocity.h>
#include <Component/Transform.h>
#include <Component/Gravity.h>

#include <PlayerController/PlayerControllerSystem.h>
#include <Component/Tween.h>

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

struct Scene
{
	EntityComponentSystem* ecs;

	void Start()
	{
#ifdef EXCEPTION_TEST
		// Can throw in 2 ways, using ErrMsg macro or manually typing

		//throw Debug::Exception<Scene>(Debug::LEVEL_ERROR, "test", ERRLG_FUNC, ERRLG_LINE);
		throw Debug::Exception<Scene>(Debug::LEVEL_ERROR, ErrMsg("test"));
#endif // EXCEPTION_TEST

		ecs = &EntityComponentSystem::GetInstance();

		////////////////////////////////////////////////
		// Initializing existing system and components
		////////////////////////////////////////////////
		// The order of registration effects the order which the components are updated
		// use late update if your component relies on other systems to update them first
		// Flipping this should give different results.
		//ecs->RegisterComponentToSystem<(component), (system)>();

		////////////////////////////////////////////////
		// Creating Entities w/ Components
		////////////////////////////////////////////////
		//Entity entt1 = ecs->CreateEntity();
		Entity entt2 = ecs->CreateEntity();

		//ecs->AddComponent(entt1, (component));

		// Manually add entities u want to be updated by a system
		//ecs->RegisterEntityToSystem<(system)>(entt1);

		// Subsequently you can destroy an entity, this will remove 
		// the entity from all systems
		//ecs->DestroyEntity(entt1);

		// You can remove a component and it will unregister
		// from all systems that require that component
		//ecs->RemoveComponent<(component)>(entt2);

		// Example of unregistering from a system
		//ecs->UnregisterEntityFromSystem<(system)>(entt1);

		Entity entt3 = ecs->CreateEntity();
		Velocity vel({ -5, 0 }, { 0, 0 });
		Transform trans({ 250, 250 }, { 100, 50 }, 0.0);
		Gravity grav({ 0, -20 });
		BoxCollider box7(trans.m_pos, 1, 1);

		Entity entt4 = ecs->CreateEntity();
		Entity entt5 = ecs->CreateEntity();
		Entity entt6 = ecs->CreateEntity();
		Transform transBox1({ -100, 2 }, { 40, 30 }, 0.0);
		Transform transBox2({ 200, 2 }, { 20, 20 }, 0.0);
		Transform transBox3({ 300, 2 }, { 30, 20 }, 0.0);
		BoxCollider box1(transBox1.m_pos, 1, 1);
		BoxCollider box2(transBox2.m_pos, 1, 1); //should collide
		BoxCollider box3(transBox3.m_pos, 1, 1); //shouldnt collide

		ecs->RegisterComponentToSystem<Velocity, PhysicsSystem>();
		ecs->RegisterComponentToSystem<Transform, PhysicsSystem>();
		ecs->RegisterComponentToSystem<Gravity, PhysicsSystem>();

		ecs->RegisterComponentToSystem<BoxCollider, CollisionSystem>();

		ecs->RegisterComponentToSystem<Velocity, DraggableObjectSystem>();
		ecs->RegisterComponentToSystem<Transform, DraggableObjectSystem>();
		ecs->RegisterComponentToSystem<Gravity, DraggableObjectSystem>();
		ecs->RegisterComponentToSystem<BoxCollider, DraggableObjectSystem>();

		ecs->AddComponent(entt3, vel);
		ecs->AddComponent(entt3, trans);
		ecs->AddComponent(entt3, grav);
		ecs->RegisterEntityToSystem<PhysicsSystem>(entt3);

		ecs->AddComponent(entt4, box1);
		ecs->AddComponent(entt4, transBox1);
		ecs->AddComponent(entt5, box2);
		ecs->AddComponent(entt5, transBox2);
		ecs->RegisterEntityToSystem<CollisionSystem>(entt4);
		ecs->RegisterEntityToSystem<CollisionSystem>(entt5);
		
		ecs->AddComponent(entt6, box3);
		ecs->AddComponent(entt6, transBox3);
		ecs->RegisterEntityToSystem<CollisionSystem>(entt6);

		ecs->AddComponent(entt3, box7);
		ecs->RegisterEntityToSystem<CollisionSystem>(entt3);
		ecs->RegisterEntityToSystem<DraggableObjectSystem>(entt3);


		Entity player = ecs->CreateEntity();
		Transform playerTrans({ 0, 0 }, { 1, 1 }, 0.0);
		Tween tween(3.0);
		tween.AddTween({ -1, 0 });
		tween.AddTween({ -1, 1 });
		tween.AddTween({ -2, 1 });
		ecs->RegisterComponentToSystem<Tween, PlayerControllerSystem>();
		ecs->AddComponent(player, playerTrans);
		ecs->AddComponent(player, tween);
		ecs->RegisterEntityToSystem<PlayerControllerSystem>(player);

#pragma region RENDERING_SYSTEM // Rendering should be last ( I think?!)

#pragma endregion // end of rendering system block
	}

	void Update()
	{
		// This should be done by app controller in main
		ecs->UpdateSystems();
	}

	void Exit()
	{
		// Example of removing systems
	}
};