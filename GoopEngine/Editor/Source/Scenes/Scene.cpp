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
}

void GE::Scenes::Scene::Unload()
{
	Audio::AudioSystem::GetInstance().StopAllChannels();
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
	of->LoadSceneJson("SceneTest");
	std::set<Entity> stackObj;
	of->LoadSceneObjects(stackObj);

	Audio::AudioSystem::GetInstance().PlaySound(Assets::AssetManager::GetInstance().GetSound("bgm1"), 0.5f, true);
}
