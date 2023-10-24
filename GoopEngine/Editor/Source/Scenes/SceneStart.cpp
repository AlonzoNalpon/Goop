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
}

void GE::Scenes::SceneStart::Init()
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
}

void GE::Scenes::SceneStart::Unload()
{
	for (auto entity : m_entities) {
		ecs->DestroyEntity(entity);
	}
}

void GE::Scenes::SceneStart::Free()
{

}
