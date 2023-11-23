/*!*********************************************************************
\file   Scene.cpp
\author loh.j@digipen.edu
\date   03-November-2023
\brief
	Contains the functionality of the main Scene class.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "Scene.h"
#include <Systems/DraggableObject/DraggableObjectSystem.h>
#include <Systems/PlayerController/PlayerControllerSystem.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>
#include <Systems/Rendering/RenderingSystem.h>
#include <Systems/Physics/CollisionSystem.h>
#include <Systems/Enemy/EnemySystem.h>
#include <Fmod/FmodSystem.h>

using namespace GE;
using namespace ECS;
using namespace Component;

void GE::Scenes::Scene::Load(std::string filepath)
{
	ecs = { &GE::ECS::EntityComponentSystem::GetInstance() };
	of = { &GE::ObjectFactory::ObjectFactory::GetInstance() };
	of->LoadSceneJson(filepath);
	GE::Systems::EnemySystem::InitTree();
}

void GE::Scenes::Scene::Init()
{
	std::set<Entity> stackObj;
	of->LoadSceneObjects(stackObj); 
}

void GE::Scenes::Scene::Unload()
{
	GE::fMOD::FmodSystem::GetInstance().StopAllSound();
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
	
	of->ClearSceneObjects();
	of->LoadSceneJson(GE::Assets::AssetManager::GetInstance().GetScene("SceneTest"));
	std::set<Entity> stackObj;
	of->LoadSceneObjects(stackObj);
	Entity gameSys = ecs->CreateEntity();
	std::vector<void*> arg{};


	Entity testPlayer = ecs->CreateEntity();
	Entity testEnemy = ecs->CreateEntity();

	GE::Component::Game GameComp{};
	ScriptInstance GameMan{ "GameManager",arg };
	GameComp.m_gameSystemScript = GameMan;
	GameComp.m_enemy = 4;
	GameComp.m_player = 3;
	GameComp.m_pauseMenu = (unsigned) - 1;

	GE::Component::Transform GSTrans{};
	std::vector<std::string> listOFScripts{ "Stats" };
	GE::Component::Scripts testPLayerScript = Scripts(listOFScripts);
	GE::Component::Scripts testEnemyScript = Scripts(listOFScripts);
	testPLayerScript.m_scriptList[0].GetFields();
	testPLayerScript.m_entityId = testPlayer;
	testEnemyScript.m_scriptList[0].GetFields();
	testEnemyScript.m_entityId = testEnemy;

	ecs->SetEntityName(gameSys, "GameSys");
	ecs->AddComponent(gameSys, GameComp);
	ecs->AddComponent(gameSys, GSTrans);

	ecs->SetEntityName(testPlayer, "Test Player");
	ecs->AddComponent(testPlayer, GSTrans);
	ecs->AddComponent(testPlayer, testPLayerScript);

	ecs->SetEntityName(testEnemy, "Test Enemy");
	ecs->AddComponent(testEnemy, GSTrans);
	ecs->AddComponent(testEnemy, testEnemyScript);
}
