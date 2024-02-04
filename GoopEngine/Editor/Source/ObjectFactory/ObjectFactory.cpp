/*!*********************************************************************
\file   ObjectFactory.cpp
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Contains the definition of the ObjectFactory singleton class, which
  encapsulates functions and data members to facilitate the creation
  of objects in the scene. The class serves as a middle-man between
  serialization and the actual entities in the scene. It is responsible
  for the creation/cloning of entities and adding of components through
  the ECS.

  On load, the ObjectFactory holds the deserialized data for the scene.
  Upon reloading, the objects are loaded from here without the need to
  deserialize again. Only when changing scenes will a full reload be
  required.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ObjectFactory.h"
#include <AssetManager/AssetManager.h>
#include <Systems/Systems.h>
#include "SerializeComponents.h"
#include <Systems/Rendering/RenderingSystem.h>
#include <Serialization/Deserializer.h>
#ifndef IMGUI_DISABLE
#include <Prefabs/PrefabManager.h>
#endif

using namespace GE::ObjectFactory;
using namespace GE::ECS;

void ObjectFactory::AddComponentsToEntity(ECS::Entity id, std::vector<rttr::variant> const& components) const
{
  for (rttr::variant const& component : components)
  {
    AddComponentToEntity(id, component);
  }
}

std::vector<rttr::variant> ObjectFactory::GetEntityComponents(ECS::Entity id) const
{
  std::vector<rttr::variant> ret;
  ECS::ComponentSignature const sig{ ECS::EntityComponentSystem::GetInstance().GetComponentSignature(id) };
  ret.reserve(sig.count()); // reserve based on number of bits true
  for (unsigned i{}; i < sig.size(); ++i)
  {
    if (sig[i])
    {
      ret.emplace_back(GetEntityComponent(id, ECS::componentTypes[i]));
    }
  }

  return ret;
}

void GE::ObjectFactory::ObjectFactory::EmptyMap()
{
  m_deserialized.clear();
}

GE::ECS::Entity GE::ObjectFactory::ObjectFactory::CreateObject(std::string name, Math::dVec3 pos, Math::dVec3 scale, Math::dVec3 rot, ECS::Entity parent)
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };
  Entity newEntity = ecs.CreateEntity();
  ecs.SetEntityName(newEntity, name);
  ecs.SetIsActiveEntity(newEntity, true);

  GE::Component::Transform trans{pos, scale, rot};
  ecs.AddComponent(newEntity, trans);

  ecs.SetParentEntity(newEntity, parent);
  if (parent != ECS::INVALID_ID) { ecs.AddChildEntity(parent, newEntity); }

  return newEntity;
}

void GE::ObjectFactory::ObjectFactory::UpdateSprite(GE::ECS::Entity entity, std::string textureName)
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };
  auto const& texManager = GE::Graphics::GraphicsEngine::GetInstance().textureManager;

  if (ecs.HasComponent<GE::Component::Sprite>(entity))
  {
    GE::Component::Sprite* entitySpriteData = ecs.GetComponent<GE::Component::Sprite>(entity);
    entitySpriteData->m_spriteData.texture = texManager.GetTextureID(GE::GoopUtils::ExtractFilename(textureName));
  }
  else
  {
    auto& gEngine = GE::Graphics::GraphicsEngine::GetInstance();
    GE::Component::Sprite sprite{ gEngine.textureManager.GetTextureID(GE::GoopUtils::ExtractFilename(textureName)) };
    ecs.AddComponent(entity, sprite);
  }
}

void ObjectFactory::CloneObject(ECS::Entity entity, ECS::Entity parent) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  Entity newEntity = ecs.CreateEntity();
  ecs.SetEntityName(newEntity, ecs.GetEntityName(entity) + " (Copy)");
  ecs.SetIsActiveEntity(newEntity, ecs.GetIsActiveEntity(entity));

  std::vector<rttr::variant> const components{ GetEntityComponents(entity) };

  AddComponentsToEntity(newEntity, components);

#ifndef IMGUI_DISABLE
  // update entity's prefab if needed
  Prefabs::PrefabManager& pm{ Prefabs::PrefabManager::GetInstance() };
  auto const entityPrefab{ pm.GetEntityPrefab(entity) };
  if (entityPrefab)
  {
    pm.AttachPrefab(newEntity, *entityPrefab);
  }
#endif

  // set parent/child
  ecs.SetParentEntity(newEntity, parent);
  if (parent != ECS::INVALID_ID) { ecs.AddChildEntity(parent, newEntity); }
  for (ECS::Entity const& child : ecs.GetChildEntities(entity))
  {
    CloneObject(child, newEntity);  // recursively clone all children
  }
}

void ObjectFactory::ClearSceneObjects()
{
  m_deserialized.clear();
}

void ObjectFactory::LoadSceneObjects()
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };

  for (auto const& [id, data] : m_deserialized)
  {
    ecs.CreateEntity({}, id, data.m_name);
    ecs.SetIsActiveEntity(const_cast<ECS::Entity&>(id), data.m_isActive);
    AddComponentsToEntity(id, data.m_components);
  }

  for (auto const& [id, data] : m_deserialized)
  {
    ecs.SetParentEntity(id, data.m_parent);

    for (ECS::Entity const& child : data.m_childEntities)
    {
      ecs.AddChildEntity(id, child);
    }
  }

#ifndef IMGUI_DISABLE
  if (Prefabs::PrefabManager::GetInstance().UpdateAllEntitiesFromPrefab())
  {
    Events::EventManager::GetInstance().Dispatch(Events::PrefabInstancesUpdatedEvent());
  }
#endif
}

void ObjectFactory::LoadSceneJson(std::string const& filename)
{
  m_deserialized = GE::Serialization::Deserializer::DeserializeScene(filename);
}

void ObjectFactory::AddComponentToEntity(ECS::Entity entity, rttr::variant const& compVar) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };
  rttr::type compType{ compVar.get_type() };
  // get underlying type if it's wrapped in a pointer
  compType = compType.is_wrapper() ? compType.get_wrapped_type().get_raw_type() : compType.is_pointer() ? compType.get_raw_type() : compType;

  if (compType == rttr::type::get<Component::Transform>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Transform*>());
  }
  else if (compType == rttr::type::get<Component::BoxCollider>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::BoxCollider*>());
  }
  else if (compType == rttr::type::get<Component::Velocity>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Velocity*>());
  }
  else if (compType == rttr::type::get<Component::Sprite>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Sprite*>());
  }
  else if (compType == rttr::type::get<Component::SpriteAnim>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::SpriteAnim*>());
  }
  else if (compType == rttr::type::get<Component::Model>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Model*>());
  }
  else if (compType == rttr::type::get<Component::Tween>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Tween*>());
  }
  else if (compType == rttr::type::get<Component::Scripts>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Scripts*>());
  }
  else if (compType == rttr::type::get<Component::Draggable>())
  {
    ecs.AddComponent(entity, Component::Draggable{});
  }
  else if (compType == rttr::type::get<Component::EnemyAI>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::EnemyAI*>());
  }
  else if (compType == rttr::type::get<Component::Text>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Text*>());
  }
  else if (compType == rttr::type::get<Component::Audio>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Audio*>());
  }
  else if (compType == rttr::type::get<Component::GE_Button>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::GE_Button*>());
  }
  else if (compType == rttr::type::get<Component::Card>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Card*>());
  }
  else if (compType == rttr::type::get<Component::Game>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Game*>());
  }
  else if (compType == rttr::type::get<Component::CardHolder>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::CardHolder*>());
  }
  else if (compType == rttr::type::get<Component::CardHolderElem>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::CardHolderElem*>());
  }
  else if (compType == rttr::type::get<Component::Emitter>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Emitter*>());
  }
  else if (compType == rttr::type::get<Component::AnimEvents>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::AnimEvents*>());
  }
  else
  {
    std::ostringstream oss{};
    oss << "Trying to add unknown component type: " << compType.get_name().to_string() << " to entity " << entity << " | Update ObjectFactory::AddComponentToEntity";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  }
}

rttr::variant ObjectFactory::GetEntityComponent(ECS::Entity id, rttr::type const& compType) const
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  if (compType == rttr::type::get<Component::Transform>())
  {
    return ecs.HasComponent<Component::Transform>(id) ? std::make_shared<Component::Transform>(*ecs.GetComponent<Component::Transform>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::BoxCollider>())
  {
    return ecs.HasComponent<Component::BoxCollider>(id) ? std::make_shared<Component::BoxCollider>(*ecs.GetComponent<Component::BoxCollider>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::Velocity>())
  {
    return ecs.HasComponent<Component::Velocity>(id) ? std::make_shared<Component::Velocity>(*ecs.GetComponent<Component::Velocity>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::Scripts>())
  {
    return ecs.HasComponent<Component::Scripts>(id) ? std::make_shared<Component::Scripts>(*ecs.GetComponent<Component::Scripts>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::Sprite>())
  {
    return ecs.HasComponent<Component::Sprite>(id) ? std::make_shared<Component::Sprite>(*ecs.GetComponent<Component::Sprite>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::SpriteAnim>())
  {
    return ecs.HasComponent<Component::SpriteAnim>(id) ? std::make_shared<Component::SpriteAnim>(*ecs.GetComponent<Component::SpriteAnim>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::Tween>())
  {
    return ecs.HasComponent<Component::Tween>(id) ? std::make_shared<Component::Tween>(*ecs.GetComponent<Component::Tween>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::EnemyAI>())
  {
    return ecs.HasComponent<Component::EnemyAI>(id) ? std::make_shared<Component::EnemyAI>(*ecs.GetComponent<Component::EnemyAI>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::Draggable>())
  {
    return ecs.HasComponent<Component::Draggable>(id) ? std::make_shared<Component::Draggable>(*ecs.GetComponent<Component::Draggable>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::Text>())
  {
    return ecs.HasComponent<Component::Text>(id) ? std::make_shared<Component::Text>(*ecs.GetComponent<Component::Text>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::Audio>())
  {
    return ecs.HasComponent<Component::Audio>(id) ? std::make_shared<Component::Audio>(*ecs.GetComponent<Component::Audio>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::GE_Button>())
  {
    return ecs.HasComponent<Component::GE_Button>(id) ? std::make_shared<Component::GE_Button>(*ecs.GetComponent<Component::GE_Button>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::Card>())
  {
    return ecs.HasComponent<Component::Card>(id) ? std::make_shared<Component::Card>(*ecs.GetComponent<Component::Card>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::CardHolder>())
  {
    return ecs.HasComponent<Component::CardHolder>(id) ? std::make_shared<Component::CardHolder>(*ecs.GetComponent<Component::CardHolder>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::CardHolderElem>())
  {
    return ecs.HasComponent<Component::CardHolderElem>(id) ? std::make_shared<Component::CardHolderElem>(*ecs.GetComponent<Component::CardHolderElem>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::Game>())
  {
    return ecs.HasComponent<Component::Game>(id) ? std::make_shared<Component::Game>(*ecs.GetComponent<Component::Game>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::Emitter>())
  {
    return ecs.HasComponent<Component::Emitter>(id) ? std::make_shared<Component::Emitter>(*ecs.GetComponent<Component::Emitter>(id)) : rttr::variant();
  }
  else if (compType == rttr::type::get<Component::AnimEvents>())
  {
    return ecs.HasComponent<Component::AnimEvents>(id) ? std::make_shared<Component::AnimEvents>(*ecs.GetComponent<Component::AnimEvents>(id)) : rttr::variant();
  }
  else
  {
    std::ostringstream oss{};
    oss << "Trying to get unsupported component type (" << compType.get_name().to_string() << ") from Entity " << id;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return rttr::variant();
  }
}

void ObjectFactory::RemoveComponentFromEntity(ECS::Entity entity, rttr::type compType) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };
  // get underlying type if it's wrapped in a pointer
  compType = compType.is_wrapper() ? compType.get_wrapped_type().get_raw_type() : compType.is_pointer() ? compType.get_raw_type() : compType;

  if (compType == rttr::type::get<Component::Transform>())
  {
    ecs.RemoveComponent<Transform>(entity);
  }
  else if (compType == rttr::type::get<Component::BoxCollider>())
  {
    ecs.RemoveComponent<BoxCollider>(entity);
  }
  else if (compType == rttr::type::get<Component::Velocity>())
  {
    ecs.RemoveComponent<Velocity>(entity);
  }
  else if (compType == rttr::type::get<Component::Sprite>())
  {
    ecs.RemoveComponent<Sprite>(entity);
  }
  else if (compType == rttr::type::get<Component::SpriteAnim>())
  {
    ecs.RemoveComponent<SpriteAnim>(entity);
  }
  else if (compType == rttr::type::get<Component::Model>())
  {
    ecs.RemoveComponent<Model>(entity);
  }
  else if (compType == rttr::type::get<Component::Tween>())
  {
    ecs.RemoveComponent<Tween>(entity);
  }
  else if (compType == rttr::type::get<Component::Scripts>())
  {
    ecs.RemoveComponent<Scripts>(entity);
  }
  else if (compType == rttr::type::get<Component::Draggable>())
  {
    ecs.RemoveComponent<Draggable>(entity);
  }
  else if (compType == rttr::type::get<Component::EnemyAI>())
  {
    ecs.RemoveComponent<EnemyAI>(entity);
  }
  else if (compType == rttr::type::get<Component::Text>())
  {
    ecs.RemoveComponent<Text>(entity);
  }
  else if (compType == rttr::type::get<Component::Audio>())
  {
    ecs.RemoveComponent<Audio>(entity);
  }
  else if (compType == rttr::type::get<Component::GE_Button>())
  {
    ecs.RemoveComponent<GE_Button>(entity);
  }
  else if (compType == rttr::type::get<Component::Card>())
  {
    ecs.RemoveComponent<Card>(entity);
  }
  else if (compType == rttr::type::get<Component::Game>())
  {
    ecs.RemoveComponent<Game>(entity);
  }
  else if (compType == rttr::type::get<Component::CardHolder>())
  {
    ecs.RemoveComponent<CardHolder>(entity);
  }
  else if (compType == rttr::type::get<Component::CardHolderElem>())
  {
    ecs.RemoveComponent<CardHolderElem>(entity);
  }
  else if (compType == rttr::type::get<Component::Emitter>())
  {
    ecs.RemoveComponent<Emitter>(entity);
  }
  else if (compType == rttr::type::get<Component::AnimEvents>())
  {
    ecs.RemoveComponent<AnimEvents>(entity);
  }
  else
  {
    std::ostringstream oss{};
    oss << "Trying to remove unknown component type: " << compType.get_name().to_string() << " to entity " << entity << " | Update ObjectFactory::RemoveComponentFromEntity";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  }
}

void ObjectFactory::RegisterComponentsAndSystems() const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  // Register components in order of COMPONENT_TYPES enum
  for (rttr::type const& compType : ECS::componentTypes)
  {
    if(compType == rttr::type::get<Component::Transform>())
      ecs.RegisterComponent<GE::Component::Transform>();
    else if (compType == rttr::type::get<Component::BoxCollider>())
      ecs.RegisterComponent<GE::Component::BoxCollider>();
    else if (compType == rttr::type::get<Component::SpriteAnim>())
      ecs.RegisterComponent<GE::Component::SpriteAnim>();
    else if (compType == rttr::type::get<Component::Sprite>())
      ecs.RegisterComponent<GE::Component::Sprite>();
    else if (compType == rttr::type::get<Component::Velocity>())
      ecs.RegisterComponent<GE::Component::Velocity>();
    else if (compType == rttr::type::get<Component::Tween>())
      ecs.RegisterComponent<GE::Component::Tween>();
    else if (compType == rttr::type::get<Component::Scripts>())
      ecs.RegisterComponent<GE::Component::Scripts>();
    else if (compType == rttr::type::get<Component::Draggable>())
      ecs.RegisterComponent<GE::Component::Draggable>();
    else if (compType == rttr::type::get<Component::EnemyAI>())
      ecs.RegisterComponent<GE::Component::EnemyAI>();
    else if (compType == rttr::type::get<Component::Text>())
      ecs.RegisterComponent<GE::Component::Text>();
    else if (compType == rttr::type::get<Component::Game>())
      ecs.RegisterComponent<GE::Component::Game>();
    else if (compType == rttr::type::get<Component::Audio>())
      ecs.RegisterComponent<GE::Component::Audio>();
    else if (compType == rttr::type::get<Component::GE_Button>())
      ecs.RegisterComponent<GE::Component::GE_Button>();
    else if (compType == rttr::type::get<Component::Card>())
      ecs.RegisterComponent<GE::Component::Card>();
    else if (compType == rttr::type::get<Component::CardHolder>())
      ecs.RegisterComponent<GE::Component::CardHolder>();
    else if (compType == rttr::type::get<Component::CardHolderElem>())
      ecs.RegisterComponent<GE::Component::CardHolderElem>();
    else if (compType == rttr::type::get<Component::Emitter>())
      ecs.RegisterComponent<GE::Component::Emitter>();
    else if (compType == rttr::type::get<Component::AnimEvents>())
      ecs.RegisterComponent<GE::Component::AnimEvents>();
    else
    {
      std::ostringstream oss{};
      oss << "Trying to register unknown component type, " << " update function: ObjectFactory::RegisterComponentsAndSystems()";
      Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
      std::cout << oss.str() << "\n";
#endif
    }
  }

  // Register systems
  std::string const systemsFile{ Assets::AssetManager::GetInstance().GetConfigData<std::string>("Systems") };
  auto const systems{ Serialization::Deserializer::DeserializeSystems(systemsFile) };

  for (auto const& [sys, components] : systems)
  {
    RegisterSystemWithType(rttr::type::get_by_name(sys), components);
  }
}

void ObjectFactory::RegisterSystemWithType(rttr::type const& systemType, std::vector<rttr::type> const& components) const
{
  ECS::EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };
  if (systemType == rttr::type::get<Systems::CollisionSystem>())
  {
    ecs.RegisterSystem<Systems::CollisionSystem>();
    RegisterComponentsToSystem<Systems::CollisionSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::PhysicsSystem>())
  {
    ecs.RegisterSystem<Systems::PhysicsSystem>();
    RegisterComponentsToSystem<Systems::PhysicsSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::DraggableObjectSystem>())
  {
    ecs.RegisterSystem<Systems::DraggableObjectSystem>();
    RegisterComponentsToSystem<Systems::DraggableObjectSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::ScriptSystem>())
  {
    ecs.RegisterSystem<Systems::ScriptSystem>();
    RegisterComponentsToSystem<Systems::ScriptSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::RenderSystem>())
  {
    ecs.RegisterSystem<Systems::RenderSystem>();
    RegisterComponentsToSystem<Systems::RenderSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::SpriteAnimSystem>())
  {
    ecs.RegisterSystem<Systems::SpriteAnimSystem>();
    RegisterComponentsToSystem<Systems::SpriteAnimSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::EnemySystem>())
  {
    ecs.RegisterSystem<Systems::EnemySystem>();
    RegisterComponentsToSystem<Systems::EnemySystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::PreRootTransformSystem>())
  {
    ecs.RegisterSystem<Systems::PreRootTransformSystem>();
    RegisterComponentsToSystem<Systems::PreRootTransformSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::PostRootTransformSystem>())
  {
    ecs.RegisterSystem<Systems::PostRootTransformSystem>();
    RegisterComponentsToSystem<Systems::PostRootTransformSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::TextRenderSystem>())
  {
    ecs.RegisterSystem<Systems::TextRenderSystem>();
    RegisterComponentsToSystem<Systems::TextRenderSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::TweenSystem>())
  {
    ecs.RegisterSystem<Systems::TweenSystem>();
    RegisterComponentsToSystem<Systems::TweenSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::AudioSystem>())
  {
    ecs.RegisterSystem<Systems::AudioSystem>();
    RegisterComponentsToSystem<Systems::AudioSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::ButtonSystem>())
  {
    ecs.RegisterSystem<Systems::ButtonSystem>();
    RegisterComponentsToSystem<Systems::ButtonSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::GameSystem>())
  {
    ecs.RegisterSystem<Systems::GameSystem>();
    RegisterComponentsToSystem<Systems::GameSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::ButtonScriptSystem>())
  {
    ecs.RegisterSystem<Systems::ButtonScriptSystem>();
    RegisterComponentsToSystem<Systems::ButtonScriptSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::CardHolderSystem>())
  {
    ecs.RegisterSystem<Systems::CardHolderSystem>();
    RegisterComponentsToSystem<Systems::CardHolderSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::ParticleSystem>())
  {
    ecs.RegisterSystem<Systems::ParticleSystem>();
    RegisterComponentsToSystem<Systems::ParticleSystem>(components);
  }
  else if (systemType == rttr::type::get<Systems::AnimEventsSystem>())
  {
    ecs.RegisterSystem<Systems::AnimEventsSystem>();
    RegisterComponentsToSystem<Systems::AnimEventsSystem>(components);
  }
  else
  {
    std::ostringstream oss{};
    oss << "Trying to register unknown system type: " << systemType.get_name().to_string();
    Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
    std::cout << oss.str() << "\n";
#endif
  }
}
