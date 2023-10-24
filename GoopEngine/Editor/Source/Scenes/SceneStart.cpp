#include <pch.h>
#include "SceneStart.h"

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

void GE::Scenes::SceneStart::Load()
{
  ecs = { &GE::ECS::EntityComponentSystem::GetInstance() };
  of = { &GE::ObjectFactory::ObjectFactory::GetInstance() };
	of->LoadSceneJson("SceneStart");
}

void GE::Scenes::SceneStart::Init()
{
	of->LoadSceneObjects(m_entities);
	//ecs->SetEntityName(background, "MainMenu");
}

void GE::Scenes::SceneStart::Unload()
{
	for (auto entity : m_entities) {
		ecs->DestroyEntity(entity);
	}
}

void GE::Scenes::SceneStart::Free()
{
	of->EmptyMap();
}
