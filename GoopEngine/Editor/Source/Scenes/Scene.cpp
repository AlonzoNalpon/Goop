/*!*********************************************************************
\file   Scene.cpp
\author loh.j\@digipen.edu
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
#include <ScriptEngine/ScriptManager.h>

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
	// All sounds stops and play calls by script now
	//GE::fMOD::FmodSystem::GetInstance().StopAllSound();
	std::set<ECS::Entity> entities = ecs->GetEntities();
	for (auto entity : entities)
	{
		// Ignore if entity is inactive
		if (ecs->GetIsActiveEntity(entity))
		{
			if (ecs->HasComponent<GE::Component::Scripts>(entity))
			{
				GE::Component::Scripts* scripts = ecs->GetComponent<GE::Component::Scripts>(entity);
				for (auto script : scripts->m_scriptList)
				{
					if (!script.m_scriptClass)
					{
						continue;
					}

					MonoMethod* onDestroy = mono_class_get_method_from_name(script.m_scriptClass, "OnDestroy", 1);
					if (onDestroy)
					{
						std::vector<void*> params = { &entity };
						mono_runtime_invoke(onDestroy, mono_gchandle_get_target(script.m_gcHandle), params.data(), nullptr);
					}
				}
			}
		}

		if (ecs->HasComponent<GE::Component::Scripts>(entity))
		{
			GE::Component::Scripts* scripts = ecs->GetComponent<GE::Component::Scripts>(entity);
			for (auto& script : scripts->m_scriptList)
			{
				if (!script.m_scriptClass)
				{
					continue;
				}
				script.FreeScript();
			}
		}
		ecs->DestroyEntity(entity);
	}
	GE::MONO::ScriptManager::GetInstance().ReloadAssembly();
}

void GE::Scenes::Scene::Free()
{
	of->EmptyMap();
}
