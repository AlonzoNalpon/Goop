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
#include <Serialization/GooStream/ObjectGooStream.h>
#include <Serialization/GooStream/PrefabGooStream.h>
#include <Systems/Rendering/RenderingSystem.h>
#include <Serialization/Deserializer.h>

using namespace GE::ObjectFactory;
using namespace GE::ECS;

#ifndef RTTR_DESERIALIZE
void ObjectFactory::CloneComponents(GE::ECS::Entity destObj, GE::ECS::Entity srcObj) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };
  ECS::ComponentSignature const sig{ ecs.GetComponentSignature(srcObj) };

  if (IsBitSet(sig, ECS::COMPONENT_TYPES::TRANSFORM))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Transform>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::BOX_COLLIDER))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::BoxCollider>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::VELOCITY))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Velocity>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::SPRITE))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Sprite>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::SPRITE_ANIM))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::SpriteAnim>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::TWEEN))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Tween>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::SCRIPTS))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Scripts>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::DRAGGABLE))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Draggable>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::ENEMY_AI))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::EnemyAI>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::TEXT))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Text>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::AUDIO))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Audio>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::GE_BUTTON))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::GE_Button>(srcObj));
  }
}

void ObjectFactory::AddComponentToObject(ECS::Entity id, ObjectData const& data) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::TRANSFORM))
  {
    ecs.AddComponent(id,
      DeserializeComponent<GE::Component::Transform>(data.m_components.at(GE::ECS::COMPONENT_TYPES::TRANSFORM)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::BOX_COLLIDER))
  {
    ecs.AddComponent(id,
      DeserializeComponent<GE::Component::BoxCollider>(data.m_components.at(GE::ECS::COMPONENT_TYPES::BOX_COLLIDER)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::VELOCITY))
  {
    ecs.AddComponent(id,
      DeserializeComponent<GE::Component::Velocity>(data.m_components.at(GE::ECS::COMPONENT_TYPES::VELOCITY)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SPRITE))
  {
    ecs.AddComponent(id,
      DeserializeComponent<GE::Component::Sprite>(data.m_components.at(GE::ECS::COMPONENT_TYPES::SPRITE)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SPRITE_ANIM))
  {
    ecs.AddComponent(id,
      DeserializeComponent<GE::Component::SpriteAnim>(data.m_components.at(GE::ECS::COMPONENT_TYPES::SPRITE_ANIM)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::TWEEN))
  {
    ecs.AddComponent(id,
      DeserializeComponent<GE::Component::Tween>(data.m_components.at(GE::ECS::COMPONENT_TYPES::TWEEN)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SCRIPTS))
  {
    ecs.AddComponent(id,
      DeserializeScripts(data.m_components.at(GE::ECS::COMPONENT_TYPES::SCRIPTS), id));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::DRAGGABLE))
  {
    ecs.AddComponent(id,Draggable());
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::ENEMY_AI))
  {
    ecs.AddComponent(id,
      DeserializeComponent<GE::Component::EnemyAI>(data.m_components.at(GE::ECS::COMPONENT_TYPES::ENEMY_AI)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::TEXT))
  {
    ecs.AddComponent(id,
      DeserializeComponent<GE::Component::Text>(data.m_components.at(GE::ECS::COMPONENT_TYPES::TEXT)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::AUDIO))
  {
    ecs.AddComponent(id,
      DeserializeComponent<GE::Component::Audio>(data.m_components.at(GE::ECS::COMPONENT_TYPES::AUDIO)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::GE_BUTTON))
  {
    ecs.AddComponent(id,
      DeserializeComponent<GE::Component::GE_Button>(data.m_components.at(GE::ECS::COMPONENT_TYPES::GE_BUTTON)));
  }
}

#else

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
}

void ObjectFactory::AddComponentsToEntity(ECS::Entity id, std::vector<rttr::variant> const& components) const
{
  for (rttr::variant const& component : components)
  {
    AddComponentToEntity(id, component);
  }
}
#endif

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
    case COMPONENT_TYPES::AUDIO:
      ecs.RegisterComponent<GE::Component::Audio>();
      break;
    case COMPONENT_TYPES::GE_BUTTON:
      ecs.RegisterComponent<GE::Component::GE_Button>();
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
#ifndef RTTR_DESERIALIZE
    Serialization::PrefabGooStream pgs{ path };
    std::pair <std::string, ObjectData> data;
    pgs.Unload(data);
    m_prefabs.emplace(std::move(data));
#else
    m_prefabs.emplace(name, Serialization::Deserializer::DeserializePrefabToVariant(path));
#endif
  }
}

GE::ECS::Entity ObjectFactory::SpawnPrefab(const std::string& key)
{
#ifndef RTTR_DESERIALIZE
  std::unordered_map<std::string, ObjectData>::const_iterator iter{ m_prefabs.find(key) };
  if (iter == m_prefabs.end())
  {
    ReloadPrefabs();
    if ((iter = m_prefabs.find(key)) == m_prefabs.end())
    {
      throw GE::Debug::Exception<ObjectFactory>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load prefab " + key));
    }
  }

  ECS::Entity const newEntity{ ECS::EntityComponentSystem::GetInstance().CreateEntity() };
  AddComponentToObject(newEntity, iter->second);

  return newEntity;
#else
  PrefabDataContainer::const_iterator iter{ m_prefabs.find(key) };
  if (iter == m_prefabs.end())
  {
    ReloadPrefabs();
    if ((iter = m_prefabs.find(key)) == m_prefabs.end())
    {
      throw GE::Debug::Exception<ObjectFactory>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load prefab " + key));
    }
  }

  ECS::Entity const newEntity{ ECS::EntityComponentSystem::GetInstance().CreateEntity() };
  AddComponentsToEntity(newEntity, iter->second.m_components);

  return newEntity;
#endif
}

void ObjectFactory::ReloadPrefabs()
{
  GoopUtils::ReloadFileData();
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
#ifndef RTTR_DESERIALIZE
  CloneComponents(newEntity, entity);
#else

  for (ECS::COMPONENT_TYPES component{ static_cast<ECS::COMPONENT_TYPES>(0) }; component < ECS::COMPONENT_TYPES::COMPONENTS_TOTAL; ++component)
  {
    rttr::variant compVar{ Serialization::Serializer::GetEntityComponent(entity, component) };
    // skip if component wasn't found
    if (!compVar.is_valid()) { continue; }

    AddComponentToEntity(newEntity, compVar);
  }

#endif

  ecs.SetParentEntity(newEntity, parent);
  if (parent != ECS::INVALID_ID) { ecs.AddChildEntity(parent, newEntity); }
  for (ECS::Entity const& child : ecs.GetChildEntities(entity))
  {
    CloneObject(child, newEntity);
  }
}

void ObjectFactory::ClearSceneObjects()
{
  m_deserialized.clear();
}

void ObjectFactory::LoadSceneObjects(std::set<GE::ECS::Entity>& map)
{
#ifndef RTTR_DESERIALIZE
  try
  {
    ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
    for (auto const& [id, data] : m_deserialized)
    {
      ecs.CreateEntity({}, id, data.m_name);
      AddComponentToObject(id, data);
    }

    // iterate through entities again and assign parent-child
    // relation based on custom IDs
    for (auto& [id, data] : m_deserialized)
    {
      ecs.SetParentEntity(id, data.m_parent);

      for (ECS::Entity const& child : data.m_childEntities)
      {
        ecs.AddChildEntity(id, child);
      }
    }
  }
  catch (GE::Debug::IExceptionBase& e)
  {
    e.LogSource();
    e.Log();
  }
#else
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };

  for (auto const& [id, data] : m_deserialized)
  {
    ecs.CreateEntity({}, id, data.m_name);
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
#endif
}

void ObjectFactory::LoadSceneJson(std::string const& filename)
{
#ifndef RTTR_DESERIALIZE
  Serialization::ObjectGooStream ogs{ GE::Assets::AssetManager::GetInstance().GetScene(filename) };
  if (ogs)
  {
    ogs.Unload(m_deserialized);
  }
#else
  m_deserialized = GE::Serialization::Deserializer::DeserializeScene(GE::Assets::AssetManager::GetInstance().GetScene(filename));
#endif
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
  case SYSTEM_TYPES::AUDIO_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::AudioSystem>();
    RegisterComponentsToSystem<Systems::AudioSystem>(sig);
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
