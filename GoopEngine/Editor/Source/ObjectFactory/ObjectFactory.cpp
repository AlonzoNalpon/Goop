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
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::MODEL))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Model>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::TWEEN))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Tween>(srcObj));
  }
  if (IsBitSet(sig, ECS::COMPONENT_TYPES::SCRIPT_HANDLER))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::ScriptHandler>(srcObj));
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
}

GE::ECS::Entity ObjectFactory::CreateObject(std::string const& name, ObjectData const& data) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  Entity newData = ecs.CreateEntity();
  ecs.SetEntityName(newData, name);

  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::TRANSFORM))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Transform>(data.m_components.at(GE::ECS::COMPONENT_TYPES::TRANSFORM)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::BOX_COLLIDER))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::BoxCollider>(data.m_components.at(GE::ECS::COMPONENT_TYPES::BOX_COLLIDER)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::VELOCITY))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Velocity>(data.m_components.at(GE::ECS::COMPONENT_TYPES::VELOCITY)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SPRITE))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Sprite>(data.m_components.at(GE::ECS::COMPONENT_TYPES::SPRITE)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SPRITE_ANIM))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::SpriteAnim>(data.m_components.at(GE::ECS::COMPONENT_TYPES::SPRITE_ANIM)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::MODEL))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Model>(data.m_components.at(GE::ECS::COMPONENT_TYPES::MODEL)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::TWEEN))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Tween>(data.m_components.at(GE::ECS::COMPONENT_TYPES::TWEEN)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SCRIPT_HANDLER))
  {
    ecs.AddComponent(newData,
      DeserializeScriptHandler(data.m_components.at(GE::ECS::COMPONENT_TYPES::SCRIPT_HANDLER), newData));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::DRAGGABLE))
  {
    ecs.AddComponent(newData,Draggable());
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::ENEMY_AI))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::EnemyAI>(data.m_components.at(GE::ECS::COMPONENT_TYPES::ENEMY_AI)));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::TEXT))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Text>(data.m_components.at(GE::ECS::COMPONENT_TYPES::TEXT)));
  }
  return newData;
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
    case COMPONENT_TYPES::MODEL:
      ecs.RegisterComponent<GE::Component::Model>();
      break;
    case COMPONENT_TYPES::VELOCITY:
      ecs.RegisterComponent<GE::Component::Velocity>();
      break;
    case COMPONENT_TYPES::TWEEN:
      ecs.RegisterComponent<GE::Component::Tween>();
      break;
    case COMPONENT_TYPES::SCRIPT_HANDLER:
      ecs.RegisterComponent<GE::Component::ScriptHandler>();
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
    default:
      std::ostringstream oss{};
      oss << "Trying to register unknown component type, " << " update function: ObjectFactory::RegisterComponentsAndSystems()";
      throw Debug::Exception<ObjectFactory>(Debug::LEVEL_WARN, ErrMsg(oss.str()));
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
  auto prefabs{ GE::Assets::AssetManager::GetInstance().GetPrefabs() };
  for (auto const& [name, path] : prefabs)
  {
    Serialization::PrefabGooStream pgs{ path };
    std::pair <std::string, ObjectData> data;
    pgs.Unload(data);
    m_prefabs.emplace(std::move(data));
  }
}

void ObjectFactory::DeserializePrefab(const std::string& filepath)
{
  GE::Serialization::PrefabGooStream pgs{ filepath };
  if (!pgs)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + filepath);
  }

  std::pair<std::string, ObjectData> prefab;
  if (!pgs.Unload(prefab))
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to unload " + filepath);
  }

  m_prefabs.emplace(std::move(prefab));
}

GE::ECS::Entity ObjectFactory::SpawnPrefab(const std::string& key) const
{
  if (m_prefabs.find(key) == m_prefabs.end())
  {
    throw GE::Debug::Exception<ObjectFactory>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load prefab " + key));
  }

  ObjectData prefab = m_prefabs.at(key);
  Entity entity = CreateObject(key, prefab);

  return entity;
}

void GE::ObjectFactory::ObjectFactory::EmptyMap()
{
  m_objects.clear();
  m_prefabs.clear();
}

void ObjectFactory::CloneObject(ECS::Entity obj, const Math::dVec2& newPos)
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  Entity newObj = ecs.CreateEntity();
  ecs.SetEntityName(newObj, ecs.GetEntityName(obj) + " (Copy)");
  CloneComponents(newObj, obj);
  Component::Transform* trans{ ecs.GetComponent<Component::Transform>(newObj) };
  if (trans) 
  {
    trans->m_worldPos = newPos;
  }
}

std::unordered_map<GE::ECS::Entity, GE::ECS::Entity> ObjectFactory::GenerateNewIDs() const
{
  GE::ECS::EntityComponentSystem& ecs{ GE::ECS::EntityComponentSystem::GetInstance() };

  std::unordered_map<GE::ECS::Entity, GE::ECS::Entity> ret{};
  GE::ECS::Entity currentID{};  // start from 0
  // assigns each entity to a new id starting from 0 with no gaps in between
  for (GE::ECS::Entity const& entity : ecs.GetEntities())
  {
    ret.emplace(entity, currentID);
    ++currentID;
  }

  return ret;
}

void ObjectFactory::ClearSceneObjects()
{
  m_objects.clear();
}

void ObjectFactory::LoadSceneObjects(std::set<GE::ECS::Entity>& map)
{
  try
  {
    ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
    // we will map our custom ID to the actual entity ID
    std::unordered_map<ECS::Entity, ECS::Entity> newIdToEntity{};
    ECS::Entity newID{};  // start from 0
    for (auto const& [name, data] : m_objects)
    {
      ECS::Entity i = CreateObject(name, data);
      map.emplace(i);
      newIdToEntity.emplace(newID, i);
      ++newID;
    }

    newID = 0;
    // iterate through entities again and assign parent-child
    // relation based on custom IDs
    for (auto& [name, data] : m_objects)
    {
      if (data.m_parent == ECS::INVALID_ID)
      {
        ecs.SetParentEntity(newIdToEntity[newID]);
      }
      else
      {
        ecs.SetParentEntity(newIdToEntity[newID], newIdToEntity[data.m_parent]);
      }

      for (ECS::Entity child : data.m_childEntities)
      {
        ecs.AddChildEntity(newIdToEntity[newID], newIdToEntity[child]);
      }
      ++newID;
    }
  }
  catch (GE::Debug::IExceptionBase& e)
  {
    e.LogSource();
    e.Log();
  }
}

void ObjectFactory::LoadSceneJson(std::string const& filename)
{
  Serialization::ObjectGooStream ogs{ GE::Assets::AssetManager::GetInstance().GetScene(filename) };
  if (ogs)
  {
    ogs.Unload(m_objects);
  }
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
  default:
    std::ostringstream oss{};
    oss << "Trying to register unknown system type, " << " update function: ObjectFactory::RegisterSystemWithEnum()";
    throw Debug::Exception<ObjectFactory>(Debug::LEVEL_WARN, ErrMsg(oss.str()));
    break;
  }
}
