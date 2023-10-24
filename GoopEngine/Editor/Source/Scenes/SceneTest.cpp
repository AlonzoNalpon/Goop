#include <pch.h>
#include "SceneTest.h"
#include "../Serialization/ObjectGooStream.h"

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

void GE::Scenes::SceneTest::MakeDraggableBox()
{
		Entity entt = ecs->CreateEntity();
		Velocity vel({ 0, 0, 0 }, { 0, 0, 0 }, 1.0, { 9.8, 9.8, 0 }, DragForce( {2, 2, 0}, 1));
		Transform trans({ 0, 0, 0 }, { 50, 50, 1 }, { 0.0, 0.0, 0.0 });
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
	of->LoadSceneJson("SceneTest");
}

void GE::Scenes::SceneTest::Init()
{
	of->LoadSceneObjects(m_entities);
	Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };

	Audio::AudioEngine::GetInstance().PlaySound(Assets::AssetManager::GetInstance().GetSound("bgm1"), 0.5f, true);

	Entity player = ecs->CreateEntity();
	m_entities.emplace(player);
	Transform playerTrans({ -350, 350, 0 }, { 150, 150, 1 }, { 0.0, 0.0, 45.0 });
	BoxCollider playerCollider(playerTrans.m_pos, 150, 150); //should collide

	Tween tween(3.0);
	tween.AddTween({ -900, 0, 0 });
	tween.AddTween({ 0, -350, 0 });
	tween.AddTween({ 350, 350, 0 });
	GE::Component::Model mdl; // model data for the player sprite
	mdl.mdlID = gEngine.GetModel();
	Sprite sprite;
	sprite.spriteData.texture = gEngine.textureManager.GetTextureID("SS_MineWorm"); 
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
	//ecs->SetIsActiveEntity(entt3, false);
}

void GE::Scenes::SceneTest::Unload()
{
	Audio::AudioEngine::GetInstance().StopSound(Assets::AssetManager::GetInstance().GetSound("bgm1"));
	for (auto entity : m_entities) {
		ecs->DestroyEntity(entity);
	}
}

void GE::Scenes::SceneTest::Free()
{
	//asset manager free images()
	of->EmptyMap();

}
