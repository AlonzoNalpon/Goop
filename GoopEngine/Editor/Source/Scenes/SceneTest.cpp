#include <pch.h>
#include "SceneTest.h"
#include <Component/Velocity.h>
#include <Component/Transform.h>
#include <Component/Sprite.h>
#include <Component/SpriteAnim.h>
#include <Component/BoxCollider.h>
#include <Component/Tween.h>
#include <Component/Model.h>
#include <Component/ScriptHandler.h>

#include <Systems/Physics/PhysicsSystem.h>
#include <Systems/Physics/CollisionSystem.h>
#include <Systems/DraggableObject/DraggableObjectSystem.h>
#include <Systems/PlayerController/PlayerControllerSystem.h>
#include <Systems/RootTransform/RootTransformSystem.h>
#include <Systems/Rendering/RenderingSystem.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

void GE::Scenes::SceneTest::MakeDraggableBox()
{
		Entity entt = ecs->CreateEntity();
		Velocity vel({ 0, 0, 0 }, { 0, 0, 0 }, 1.0, { 9.8, 9.8, 0 }, DragForce( {2, 2, 0}, 1));
		Transform trans({ 0, 0, 0 }, { 50, 50, 1 }, 0.0);
		BoxCollider box(trans.m_pos, 50, 50);

		ecs->AddComponent(entt, vel);
		ecs->AddComponent(entt, trans);
		ecs->AddComponent(entt, box);
		ecs->RegisterEntityToSystem<PhysicsSystem>(entt);
		ecs->RegisterEntityToSystem<CollisionSystem>(entt);
		ecs->RegisterEntityToSystem<DraggableObjectSystem>(entt);
}

void GE::Scenes::SceneTest::Load()
{
	ecs = { &GE::ECS::EntityComponentSystem::GetInstance() };
	of = { &GE::ObjectFactory::ObjectFactory::GetInstance() };
}

void GE::Scenes::SceneTest::Init()
{
	of->SpawnPrefab("Background");
	MakeDraggableBox();
	Entity entt2 = ecs->CreateEntity();
	Velocity vel({ 0, 0 , 0}, { 0, 0, 0 }, 1.0, { 9.8, 9.8, 0 }, DragForce({ 1, 0, 0 }));
	Transform trans({ 250, 250, 0 }, { 100, 50, 1 }, 0.0);
	BoxCollider box(trans.m_pos, 100, 50);

	ecs->RegisterSystem<RootTransformSystem>();

	Entity entt3 = ecs->CreateEntity();
	Entity entt4 = ecs->CreateEntity();
	Transform transBox2({ 200, 2, 0 }, { 20, 20, 1 }, 0.0);
	Transform transBox3({ 300, 2, 0 }, { 30, 20, 1 }, 0.0);
	BoxCollider box2(transBox2.m_pos, 20, 20); //should collide
	BoxCollider box3(transBox3.m_pos, 30, 20); //shouldnt collide

	ecs->AddComponent(entt2, vel);
	ecs->AddComponent(entt2, trans);
	ecs->AddComponent(entt2, box);
	ecs->RegisterEntityToSystem<PhysicsSystem>(entt2);
	ecs->RegisterEntityToSystem<CollisionSystem>(entt2);
	//ecs->RegisterEntityToSystem<DraggableObjectSystem>(entt2);

	vel.AddForce({ 100, 0, 0}, 300.0);

	ecs->AddComponent(entt3, box2);
	ecs->AddComponent(entt3, transBox2);
	ecs->RegisterEntityToSystem<CollisionSystem>(entt3);

	ecs->AddComponent(entt4, box3);
	ecs->AddComponent(entt4, transBox3);
	ecs->RegisterEntityToSystem<CollisionSystem>(entt4);

	Entity worm = GE::ObjectFactory::ObjectFactory::GetInstance().SpawnPrefab("MineWorm");
	ecs->SetEntityName(worm, "MineWorm");

	Entity player = ecs->CreateEntity();
	Transform playerTrans({ -350, 350, 0 }, { 150, 150, 1 }, 0.0);
	BoxCollider playerCollider(playerTrans.m_pos, 150, 150); //should collide

	Tween tween(3.0);
	tween.AddTween({ 0, 0, 0 });
	tween.AddTween({ 0, -350, 0 });
	tween.AddTween({ 350, 350, 0 });
	Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };
	GE::Component::Model mdl; // model data for the player sprite
	mdl.mdlID = gEngine.GetModel();
	Sprite sprite;
	sprite.spriteData.texture = gEngine.textureManager.GetTextureID("MineWorm");
	GE::Component::SpriteAnim anim;

	std::vector <std::pair<std::string, std::string>> playerScripts{ {"GoopScripts", "Player"} };

	GE::Component::ScriptHandler scriptHan = ScriptHandler(playerScripts, player);

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
	ecs->SetEntityName(player, "Player");
	ecs->SetIsActiveEntity(entt3, false);
}

void GE::Scenes::SceneTest::Unload()
{
}

void GE::Scenes::SceneTest::Free()
{
}
