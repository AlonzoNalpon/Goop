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
#include <Systems/ScriptSystem/ScriptSystem.h>
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
	of->LoadSceneObjects(); 
}

void GE::Scenes::Scene::Unload()
{
	GE::fMOD::FmodSystem::GetInstance().StopAllSound();
	std::set<ECS::Entity> entities = ecs->GetEntities();
	for (auto entity : entities)
	{
		if (ecs->HasComponent<GE::Component::Scripts>(entity))
		{
			GE::Component::Scripts* scripts = ecs->GetComponent<GE::Component::Scripts>(entity);
			for (auto script : scripts->m_scriptList)
			{
				MonoMethod* onDestroy = mono_class_get_method_from_name(script.m_scriptClass, "OnDestroy", 1);
				if (onDestroy)
				{
					std::vector<void*> params = { &entity };
					mono_runtime_invoke(onDestroy, mono_gchandle_get_target(script.m_gcHandle), params.data(), nullptr);
				}
			}
		}
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
	of->LoadSceneObjects();
	Entity gameSys = ecs->CreateEntity();
	std::vector<void*> arg{};


	Entity testPlayer = ecs->CreateEntity();
	Entity testEnemy = ecs->CreateEntity();

	GE::Component::Game GameComp{};
	ScriptInstance GameMan{ "GameManager",arg };
	GameComp.m_gameSystemScript = GameMan;
	GameComp.m_enemy = testEnemy;
	GameComp.m_player = testPlayer;
	GameComp.m_pauseMenu = (unsigned) - 1;

	GE::Component::Transform GSTrans{};
	std::vector<std::string> listOFScripts{ "Stats" };
	GE::Component::Scripts testPLayerScript = Scripts(listOFScripts);
	GE::Component::Scripts testEnemyScript = Scripts(listOFScripts);

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
