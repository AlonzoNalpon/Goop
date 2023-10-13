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
#include <Component/ScriptHandler.h>

#include <Systems/Rendering/RenderingSystem.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>
#include <Graphics/GraphicsEngine.h>
#include <Component/Model.h>
#include <Component/Sprite.h>
#include <Component/SpriteAnim.h>
#include <ObjectFactory/ObjectFactory.h>

#include <Component/Root.h>
#include <Systems/RootTransform/RootTransformSystem.h>
using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

struct Scene
{
	EntityComponentSystem* ecs;

	void MakeDraggableBox()
	{
		Entity entt3 = ecs->CreateEntity();
		Velocity vel({ 0, 0 }, { 0, 0 });
		Transform trans({ 0, 0 }, { 50, 50 }, 0.0);
		Gravity grav({ 0, 0 });
		BoxCollider box7(trans.m_pos, 1, 1);

		ecs->AddComponent(entt3, vel);
		ecs->AddComponent(entt3, trans);
		ecs->AddComponent(entt3, grav);
		ecs->RegisterEntityToSystem<PhysicsSystem>(entt3);

		ecs->AddComponent(entt3, box7);
		ecs->RegisterEntityToSystem<CollisionSystem>(entt3);
		ecs->RegisterEntityToSystem<DraggableObjectSystem>(entt3);
	}

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
		//Entity entt2 = ecs->CreateEntity();

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
		GE::ObjectFactory::ObjectFactory& of{ GE::ObjectFactory::ObjectFactory::GetInstance() };
		of.SpawnPrefab("Background");
		MakeDraggableBox();

		Entity entt3 = ecs->CreateEntity();
		Velocity vel({ 0, 0 }, { 0, 0 });
		Transform trans({ 250, 250 }, { 100, 50 }, 0.0);
		Gravity grav({ 0, -20 });
		BoxCollider box7(trans.m_pos, 1, 1);

		Entity entt5 = ecs->CreateEntity();
		Entity entt6 = ecs->CreateEntity();
		Transform transBox2({ 200, 2 }, { 20, 20 }, 0.0);
		Transform transBox3({ 300, 2 }, { 30, 20 }, 0.0);
		BoxCollider box2(transBox2.m_pos, 1, 1); //should collide
		BoxCollider box3(transBox3.m_pos, 1, 1); //shouldnt collide

		ecs->AddComponent(entt3, vel);
		ecs->AddComponent(entt3, trans);
		ecs->AddComponent(entt3, grav);
		ecs->RegisterEntityToSystem<PhysicsSystem>(entt3);

		ecs->AddComponent(entt5, box2);
		ecs->AddComponent(entt5, transBox2);
		ecs->RegisterEntityToSystem<CollisionSystem>(entt5);
		
		ecs->AddComponent(entt6, box3);
		ecs->AddComponent(entt6, transBox3);
		ecs->RegisterEntityToSystem<CollisionSystem>(entt6);

		ecs->AddComponent(entt3, box7);
		ecs->RegisterEntityToSystem<CollisionSystem>(entt3);
		//ecs->RegisterEntityToSystem<DraggableObjectSystem>(entt3);

		Entity player = ecs->CreateEntity();
		Transform playerTrans({ -350, 350 }, { 150, 150 }, 0.0);
		BoxCollider playerCollider(playerTrans.m_pos, 1, 1); //should collide
		
		Tween tween(3.0);
		tween.AddTween({ 0, 0 });
		tween.AddTween({ 0, -350 });
		tween.AddTween({ 350, 350 });
		Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };
		GE::Component::Model mdl; // model data for the player sprite
		mdl.mdlID = gEngine.GetModel();
		Sprite sprite;
		sprite.spriteData.texture = gEngine.textureManager.GetTextureID("MineWorm");
		GE::Component::SpriteAnim anim;

		GE::Component::ScriptHandler scriptHan = ScriptHandler({ {"GoopScripts","Player"} }, player);

		playerTrans.m_children.insert(entt3);
		playerTrans.m_children.insert(entt5);
		// This is bad code but temporary to properly assign children.
		// this should not be used once we have proper scene node loading
		auto childTrans = ecs->GetComponent<Transform>(entt3);
		childTrans->m_parent = player;
		childTrans = ecs->GetComponent<Transform>(entt5);
		childTrans->m_parent = player;
		ecs->AddComponent(player, playerTrans);
		ecs->AddComponent(player, tween);
		ecs->AddComponent(player, mdl);
		ecs->AddComponent(player, sprite);
		ecs->AddComponent(player, anim);
		ecs->AddComponent(player, playerCollider);
		ecs->AddComponent(player, scriptHan);
		ecs->RegisterEntityToSystem<PlayerControllerSystem>(player);
		ecs->RegisterEntityToSystem<RenderSystem>(player);
		ecs->RegisterEntityToSystem<CollisionSystem>(player);
		ecs->RegisterEntityToSystem<SpriteAnimSystem>(player);
		GE::ObjectFactory::ObjectFactory::GetInstance().SpawnPrefab("MineWorm");

		Root root;
		ecs->AddComponent(player, root);
		ecs->RegisterComponentToSystem<Root, RootTransformSystem>();
		ecs->RegisterSystem<RootTransformSystem>();
		ecs->RegisterEntityToSystem<RootTransformSystem>(player);
	}

	void Update()
	{
		ecs->UpdateSystems();
		// This should be done by app controller in main
	}

	void Exit()
	{
		// Example of removing systems
	}
};