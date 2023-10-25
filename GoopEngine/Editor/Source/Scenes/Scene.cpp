#include <pch.h>
#include "Scene.h"

#include <Systems/DraggableObject/DraggableObjectSystem.h>
#include <Systems/PlayerController/PlayerControllerSystem.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>
#include <Systems/Rendering/RenderingSystem.h>
#include <Systems/Physics/CollisionSystem.h>

using namespace GE;
using namespace ECS;
using namespace Component;

void GE::Scenes::Scene::Load(std::string scene_name)
{
	ecs = { &GE::ECS::EntityComponentSystem::GetInstance() };
	of = { &GE::ObjectFactory::ObjectFactory::GetInstance() };
	of->LoadSceneJson(scene_name);
}

void GE::Scenes::Scene::Init()
{
	of->LoadSceneObjects(m_entities);
}

void GE::Scenes::Scene::Unload()
{
	for (auto entity : m_entities) {
		ecs->DestroyEntity(entity);
	}
	m_entities.clear();
}

void GE::Scenes::Scene::Free()
{
	of->EmptyMap();
}

void GE::Scenes::Scene::TestScene()
{
	of->LoadSceneJson("SceneTest");
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
	ecs->RegisterEntityToSystem<GE::Systems::PlayerControllerSystem>(player);
	ecs->RegisterEntityToSystem<GE::Systems::RenderSystem>(player);
	ecs->RegisterEntityToSystem<GE::Systems::CollisionSystem>(player);
	ecs->RegisterEntityToSystem<GE::Systems::SpriteAnimSystem>(player);
	ecs->SetEntityName(player, "Player");
}
