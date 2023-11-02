#include <pch.h>
#include "Scene.h"


#include <Systems/DraggableObject/DraggableObjectSystem.h>
#include <Systems/PlayerController/PlayerControllerSystem.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>
#include <Systems/Rendering/RenderingSystem.h>
#include <Systems/Physics/CollisionSystem.h>
#include <Systems/Enemy/EnemySystem.h>

using namespace GE;
using namespace ECS;
using namespace Component;

void GE::Scenes::Scene::Load(std::string scene_name)
{
	ecs = { &GE::ECS::EntityComponentSystem::GetInstance() };
	of = { &GE::ObjectFactory::ObjectFactory::GetInstance() };
	of->LoadSceneJson(scene_name);
	GE::Systems::EnemySystem::InitTree();
}

void GE::Scenes::Scene::Init()
{
	std::set<Entity> stackObj;
	of->LoadSceneObjects(stackObj); 
	//Audio::AudioEngine::GetInstance().PlaySound(Assets::AssetManager::GetInstance().GetSound("bgm1"), 0.5f, true);
}

void GE::Scenes::Scene::Unload()
{
	Audio::AudioEngine::GetInstance().StopAllChannels();
	std::set<ECS::Entity> entities = ecs->GetEntities();
	for (auto entity : entities)
	{
		ecs->DestroyEntity(entity);
	}
}

void GE::Scenes::Scene::Free()
{
	of->EmptyMap();
}

void GE::Scenes::Scene::TestScene()
{
	// This is a hard coded function for us to test stuff without having to 
	// write it to json file or serialize
	of->LoadSceneJson("SceneTest");
	std::set<Entity> stackObj;
	of->LoadSceneObjects(stackObj);
	Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };

	Audio::AudioEngine::GetInstance().PlaySound(Assets::AssetManager::GetInstance().GetSound("bgm1"), 0.5f, true);

	Entity entt = ecs->CreateEntity();
	Transform trans{};
	trans.m_scale = { 1, 1, 1 };
	BoxCollider col{};
	trans.m_worldPos.x = 200;
	col.m_width = 150;
	col.m_height = 150;
#ifndef NO_IMGUI
	col.m_render = true;
#endif
	ecs->AddComponent(entt, trans);
	ecs->AddComponent(entt, col);

	//entt = ecs->CreateEntity();
	//trans.m_pos.x = 200;
	//col.m_width = 150;
	//col.m_height = 150;
	//col.m_render = true;
	//ecs->AddComponent(entt, trans);
	//ecs->AddComponent(entt, col);

	Entity player = ecs->CreateEntity();
	Transform playerTrans({ -350, 350, 0 }, { 1, 1, 1 }, { 0.0, 0.0, 0.0 });
	BoxCollider playerCollider(playerTrans.m_pos, 150, 150); //should collide

	Tween tween(3.0);
	tween.AddTween({ -900, 0, 0 });
	tween.AddTween({ 0, -350, 0 });
	tween.AddTween({ 350, 350, 0 });
	GE::Component::Model mdl{}; // model data for the player sprite
	mdl.m_mdlID = gEngine.GetModel();
	Sprite sprite;
	sprite.m_spriteData.texture = gEngine.textureManager.GetTextureID("SS_MineWorm");
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
	ecs->SetEntityName(player, "Player");

	GE::Systems::EnemySystem::SetPlayerID(player);

	Entity enemy = ecs->CreateEntity();

	Transform enemyTrans({ 100, 100, 0 }, { 1, 1, 1 }, { 0.0, 0.0,0.0 });
	EnemyAI newAI = EnemyAI(enemy, GE::AI::TreeCache());
	newAI.m_enemyTreeCache.m_nodeCacheStack.push_front(GE::AI::NodeCache(0, 0, GE::AI::NODE_STATES::STATE_NEW));

	ecs->AddComponent(enemy, newAI);
	ecs->AddComponent(enemy, enemyTrans);
	ecs->SetEntityName(enemy, "Enemy");
	ecs->AddComponent(enemy, mdl);
	ecs->AddComponent(enemy, sprite);
	ecs->AddComponent(enemy, anim);

}
