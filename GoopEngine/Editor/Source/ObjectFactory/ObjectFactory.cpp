/*!*********************************************************************
\file   ObjectFactory.cpp
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Parses in data and stores it in a map to be used in the future.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ObjectFactory.h"
#include <AssetManager/AssetManager.h>
#include <Systems/Systems.h>
#include "SerializeComponents.h"
#include <Systems/Rendering/RenderingSystem.h>
#include <Serialization/Deserializer.h>
#ifndef NO_IMGUI
#include <PrefabManager/PrefabManager.h>
#endif

using namespace GE::ObjectFactory;
using namespace GE::ECS;

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
  else if (compType == rttr::type::get<Component::Anchor>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Anchor*>());
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
  else
  {
    std::ostringstream oss{};
    oss << "Trying to add unknown component type: " << compType.get_name().to_string() << " to entity " << entity << " | Update ObjectFactory::AddComponentToEntity";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  }
}

void ObjectFactory::AddComponentsToEntity(ECS::Entity id, std::vector<rttr::variant> const& components) const
{
  for (rttr::variant const& component : components)
  {
    AddComponentToEntity(id, component);
  }
}

void ObjectFactory::RegisterComponentsAndSystems() const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  // Register components in order of COMPONENT_TYPES enum
  for (auto const& elem : GE::ECS::componentsToString)
  {
    switch (elem.first)
    {
    case COMPONENT_TYPES::TRANSFORM:
      ecs.RegisterComponent<GE::Component::Transform>();
      break;
    case COMPONENT_TYPES::BOX_COLLIDER:
      ecs.RegisterComponent<GE::Component::BoxCollider>();
      break;
    case COMPONENT_TYPES::SPRITE_ANIM:
      ecs.RegisterComponent<GE::Component::SpriteAnim>();
      break;
    case COMPONENT_TYPES::SPRITE:
      ecs.RegisterComponent<GE::Component::Sprite>();
      break;
    case COMPONENT_TYPES::VELOCITY:
      ecs.RegisterComponent<GE::Component::Velocity>();
      break;
    case COMPONENT_TYPES::TWEEN:
      ecs.RegisterComponent<GE::Component::Tween>();
      break;
    case COMPONENT_TYPES::SCRIPTS:
      ecs.RegisterComponent<GE::Component::Scripts>();
      break;
    case COMPONENT_TYPES::DRAGGABLE:
      ecs.RegisterComponent<GE::Component::Draggable>();
      break;
    case COMPONENT_TYPES::ENEMY_AI:
      ecs.RegisterComponent<GE::Component::EnemyAI>();
      break;
    case COMPONENT_TYPES::TEXT:
      ecs.RegisterComponent<GE::Component::Text>();
      break;
    case COMPONENT_TYPES::GAME:
      ecs.RegisterComponent<GE::Component::Game>();
      break;
    case COMPONENT_TYPES::AUDIO:
      ecs.RegisterComponent<GE::Component::Audio>();
      break;
    case COMPONENT_TYPES::GE_BUTTON:
      ecs.RegisterComponent<GE::Component::GE_Button>();
      break;
    case COMPONENT_TYPES::ANCHOR:
      ecs.RegisterComponent<GE::Component::Anchor>();
      break;
    case COMPONENT_TYPES::CARD:
      ecs.RegisterComponent<GE::Component::Card>();
      break;
    case COMPONENT_TYPES::CARD_HOLDER:
      ecs.RegisterComponent<GE::Component::CardHolder>();
      break;
    case COMPONENT_TYPES::CARD_HOLDER_ELEM:
      ecs.RegisterComponent<GE::Component::CardHolderElem>();
      break;
    default:
      std::ostringstream oss{};
      oss << "Trying to register unknown component type, " << " update function: ObjectFactory::RegisterComponentsAndSystems()";
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      break;
    }
  }

  // Register systems
  std::string const systemsFile{ Assets::AssetManager::GetInstance().GetConfigData<std::string>("Systems") };
  std::vector<std::pair<std::string, ECS::ComponentSignature>> const systems{ Serialization::Deserializer::DeserializeSystems(systemsFile) };

  for (std::pair<std::string, ECS::ComponentSignature> const& elem : systems)
  {
    std::unordered_map<std::string, ECS::SYSTEM_TYPES>::const_iterator iter{ ECS::stringToSystems.find(elem.first) };
    if (iter == stringToSystems.cend())
    {
      throw Debug::Exception<std::ifstream>(Debug::LEVEL_ERROR, ErrMsg("Unable to register system " + elem.first));
    }
    RegisterSystemWithEnum(iter->second, elem.second);
  }
}

void ObjectFactory::LoadPrefabsFromFile()
{
  auto const& prefabs{ GE::Assets::AssetManager::GetInstance().GetPrefabs() };
  for (auto const& [name, path] : prefabs)
  {
    m_prefabs.emplace(name, Serialization::Deserializer::DeserializePrefabToVariant(path));
  }
}

GE::ECS::Entity ObjectFactory::SpawnPrefab(const std::string& key)
{
  PrefabDataContainer::const_iterator iter{ m_prefabs.find(key) };
  if (iter == m_prefabs.end())
  {
    ReloadPrefabs();
    if ((iter = m_prefabs.find(key)) == m_prefabs.end())
    {
      throw GE::Debug::Exception<ObjectFactory>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load prefab " + key));
    }
  }
  auto& ecs = ECS::EntityComponentSystem::GetInstance();

  ECS::Entity newEntity{ ecs.CreateEntity() };
  AddComponentsToEntity(newEntity, iter->second.m_components);
#ifndef NO_IMGUI
  Prefabs::PrefabManager::GetInstance().AttachPrefab(newEntity, key);
#endif

  ecs.SetEntityName(newEntity, key);
  return newEntity;
}

VariantPrefab const& ObjectFactory::GetVariantPrefab(std::string const& name) const
{
  PrefabDataContainer::const_iterator ret{ m_prefabs.find(name) };
  if (ret == m_prefabs.cend()) { throw Debug::Exception<ObjectFactory>(Debug::LEVEL_ERROR, ErrMsg("Unable to find prefab with name: " + name)); }

  return ret->second;
}

void ObjectFactory::ReloadPrefabs()
{
  m_prefabs.clear();
  LoadPrefabsFromFile();
}

void GE::ObjectFactory::ObjectFactory::EmptyMap()
{
  m_deserialized.clear();
  m_prefabs.clear();
}

void ObjectFactory::CloneObject(ECS::Entity entity, ECS::Entity parent) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  Entity newEntity = ecs.CreateEntity();
  ecs.SetEntityName(newEntity, ecs.GetEntityName(entity) + " (Copy)");
  ecs.SetIsActiveEntity(newEntity, ecs.GetIsActiveEntity(entity));

  for (ECS::COMPONENT_TYPES component{ static_cast<ECS::COMPONENT_TYPES>(0) }; component < ECS::COMPONENT_TYPES::COMPONENTS_TOTAL; ++component)
  {
    rttr::variant compVar{ Serialization::Serializer::GetEntityComponent(entity, component) };
    // skip if component wasn't found
    if (!compVar.is_valid()) { continue; }

    AddComponentToEntity(newEntity, compVar);
  }

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

void ObjectFactory::LoadSceneObjects(std::set<GE::ECS::Entity>& map)
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
}

void ObjectFactory::LoadSceneJson(std::string const& filename)
{
  m_deserialized = GE::Serialization::Deserializer::DeserializeScene(filename);
}

void ObjectFactory::RegisterSystemWithEnum(ECS::SYSTEM_TYPES name, ECS::ComponentSignature sig) const
{
  switch (name)
  {
  case SYSTEM_TYPES::COLLISION:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::CollisionSystem>();
    RegisterComponentsToSystem<Systems::CollisionSystem>(sig);
    break;
  case SYSTEM_TYPES::DRAGGABLE_OBJECT:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::DraggableObjectSystem>();
    RegisterComponentsToSystem<Systems::DraggableObjectSystem>(sig);
    break;
  case SYSTEM_TYPES::PHYSICS:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::PhysicsSystem>();
    RegisterComponentsToSystem<Systems::PhysicsSystem>(sig);
    break;
  case SYSTEM_TYPES::PLAYER_CONTROLLER:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::PlayerControllerSystem>();
    RegisterComponentsToSystem<Systems::PlayerControllerSystem>(sig);
    break;
  case SYSTEM_TYPES::RENDERING:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::RenderSystem>();
    RegisterComponentsToSystem<Systems::RenderSystem>(sig);
    break;
  case SYSTEM_TYPES::SPRITE_ANIM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::SpriteAnimSystem>();
    RegisterComponentsToSystem<Systems::SpriteAnimSystem>(sig);
    break;  
  case SYSTEM_TYPES::POST_ROOT_TRANSFORM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::PostRootTransformSystem>();
    RegisterComponentsToSystem<Systems::PostRootTransformSystem>(sig);
    break;
  case SYSTEM_TYPES::PRE_ROOT_TRANSFORM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::PreRootTransformSystem>();
    RegisterComponentsToSystem<Systems::PreRootTransformSystem>(sig);
  case SYSTEM_TYPES::ENEMY_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::EnemySystem>();
    RegisterComponentsToSystem<Systems::EnemySystem>(sig);
    break;
  case SYSTEM_TYPES::TEXT_RENDER:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::TextRenderSystem>();
    RegisterComponentsToSystem<Systems::TextRenderSystem>(sig);
    break;
  case SYSTEM_TYPES::TWEEN_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::TweenSystem>();
    RegisterComponentsToSystem<Systems::TweenSystem>(sig);
    break;
  case SYSTEM_TYPES::GAME_SYSTEMS:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::GameSystem>();
    RegisterComponentsToSystem<Systems::GameSystem>(sig);
    break;
  case SYSTEM_TYPES::AUDIO_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::AudioSystem>();
    RegisterComponentsToSystem<Systems::AudioSystem>(sig);
    break;
  case SYSTEM_TYPES::OBJECT_ANCHOR_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::ObjectAnchorSystem>();
    RegisterComponentsToSystem<Systems::ObjectAnchorSystem>(sig);
    break;
  case SYSTEM_TYPES::BUTTON_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::ButtonSystem>();
    RegisterComponentsToSystem<Systems::ButtonSystem>(sig);
    break;
  default:
    std::ostringstream oss{};
    oss << "Trying to register unknown system type, " << " update function: ObjectFactory::RegisterSystemWithEnum()";
    throw Debug::Exception<ObjectFactory>(Debug::LEVEL_WARN, ErrMsg(oss.str()));
    break;
  }
}
