#include <pch.h>
#include "SceneTest.h"


using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

void GE::Scenes::SceneTest::MakeDraggableBox()
{
		Entity entt = ecs->CreateEntity();
		m_entities.emplace(std::ref(entt));

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
}

void GE::Scenes::SceneTest::Init()
{
	Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };

	Entity background = ecs->CreateEntity();
	m_entities.emplace(std::ref(background));
	
	Transform trans9({ 0, 0, 0 }, { 1600, 900, 1 }, { 0.0, 0.0, 0.0 });
	Sprite sprite9;
	sprite9.spriteData.texture = gEngine.textureManager.GetTextureID("Background");
	GE::Component::Model mdl9; // model data for the player sprite
	mdl9.mdlID = gEngine.GetModel();
	ecs->AddComponent(background, trans9);
	ecs->AddComponent(background, sprite9);
	ecs->AddComponent(background, mdl9);
	ecs->RegisterEntityToSystem<RenderSystem>(background);
	ecs->SetEntityName(background, "Background");

	Audio::AudioEngine::GetInstance().PlaySound(Assets::AssetManager::GetInstance().GetSound("bgm1"), 0.5f, true);

	Entity player = ecs->CreateEntity();
	Transform playerTrans({ -350, 350, 0 }, { 150, 150, 1 }, { 0.0, 0.0, 45.0 });
	m_entities.emplace(std::ref(player));
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
}
