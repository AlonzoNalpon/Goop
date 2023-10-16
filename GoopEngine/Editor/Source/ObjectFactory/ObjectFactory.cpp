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

#include <Systems/Physics/PhysicsSystem.h>
#include <Systems/Physics/CollisionSystem.h>
#include <Systems/DraggableObject/DraggableObjectSystem.h>
#include <Systems/Rendering/RenderingSystem.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>
#include <Systems/PlayerController/PlayerControllerSystem.h>
#include <Systems/RootTransform/RootTransformSystem.h>

#include "SerializeComponents.h"
#include "../Serialization/ObjectGooStream.h"
#include "../Serialization/PrefabGooStream.h"
#include "../Systems/Rendering/RenderingSystem.h"

using namespace GE::ObjectFactory;
using namespace GE::ECS;

void GE::ObjectFactory::ObjectFactory::Init()
{
  RegisterComponentsAndSystems();
  LoadPrefabsFromFile();
}

GE::ECS::SystemSignature ObjectFactory::GetObjectSystemSignature(GE::ECS::Entity obj) const
{
  GE::ECS::SystemSignature sig{};

  SetBitIfFound<Systems::PhysicsSystem>(obj, sig, ECS::SYSTEM_TYPES::PHYSICS);
  SetBitIfFound<Systems::DraggableObjectSystem>(obj, sig, ECS::SYSTEM_TYPES::DRAGGABLE_OBJECT);
  SetBitIfFound<Systems::RenderSystem>(obj, sig, ECS::SYSTEM_TYPES::RENDERING);
  SetBitIfFound<Systems::CollisionSystem>(obj, sig, ECS::SYSTEM_TYPES::COLLISION);
  SetBitIfFound<Systems::PlayerControllerSystem>(obj, sig, ECS::SYSTEM_TYPES::PLAYER_CONTROLLER);
  SetBitIfFound<Systems::RootTransformSystem>(obj, sig, ECS::SYSTEM_TYPES::ROOT_TRANSFORM);

  return sig;
}

void ObjectFactory::RegisterObjectToSystems(GE::ECS::Entity object, ECS::SystemSignature signature) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };
  
  if (IsBitSet(signature, SYSTEM_TYPES::PHYSICS))
    ecs.RegisterEntityToSystem<GE::Systems::PhysicsSystem>(object);
  if (IsBitSet(signature, SYSTEM_TYPES::COLLISION))
    ecs.RegisterEntityToSystem<GE::Systems::CollisionSystem>(object);
  if (IsBitSet(signature, SYSTEM_TYPES::DRAGGABLE_OBJECT))
    ecs.RegisterEntityToSystem<GE::Systems::DraggableObjectSystem>(object);
  if (IsBitSet(signature, SYSTEM_TYPES::PLAYER_CONTROLLER))
    ecs.RegisterEntityToSystem<GE::Systems::PlayerControllerSystem>(object);
  if (IsBitSet(signature, SYSTEM_TYPES::RENDERING))
    ecs.RegisterEntityToSystem<GE::Systems::RenderSystem>(object);
  if (IsBitSet(signature, SYSTEM_TYPES::SPRITE_ANIM))
    ecs.RegisterEntityToSystem<GE::Systems::SpriteAnimSystem>(object);
  if (IsBitSet(signature, SYSTEM_TYPES::ROOT_TRANSFORM))
    ecs.RegisterEntityToSystem<GE::Systems::RootTransformSystem>(object);
}

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
}

GE::ECS::Entity ObjectFactory::CreateObject(ObjectData data) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  Entity newData = ecs.CreateEntity();

  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::TRANSFORM))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Transform>(data.m_components[GE::ECS::COMPONENT_TYPES::TRANSFORM]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::BOX_COLLIDER))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::BoxCollider>(data.m_components[GE::ECS::COMPONENT_TYPES::BOX_COLLIDER]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::VELOCITY))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Velocity>(data.m_components[GE::ECS::COMPONENT_TYPES::VELOCITY]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SPRITE))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Sprite>(data.m_components[GE::ECS::COMPONENT_TYPES::SPRITE]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SPRITE_ANIM))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::SpriteAnim>(data.m_components[GE::ECS::COMPONENT_TYPES::SPRITE_ANIM]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::MODEL))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Model>(data.m_components[GE::ECS::COMPONENT_TYPES::MODEL]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::TWEEN))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Tween>(data.m_components[GE::ECS::COMPONENT_TYPES::TWEEN]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SCRIPT_HANDLER))
  {
    //ecs.AddComponent(newData,
    //  DeserializeComponent<GE::Component::ScriptHandler>(data.m_components[GE::ECS::COMPONENT_TYPES::SCRIPT_HANDLER]));
  }

  RegisterObjectToSystems(newData, data.m_systemSignature);

  return newData;
}

void ObjectFactory::LoadPrefabsFromFile()
{
  const char* prefabsFile{ Assets::AssetManager::GetInstance().GetConfigData<const char*>("Prefabs").value() };
  std::ifstream ifs{ prefabsFile };
  if (!ifs)
  {
    std::ostringstream oss{};
    oss << "Unable to open" << prefabsFile;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  }

  std::string filepath;
  std::string prefabFile;
  ifs >> filepath;
  while (ifs >> prefabFile)
  {
    DeserializePrefab(filepath + prefabFile);
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
    default:
      throw Debug::Exception<ObjectFactory>(Debug::LEVEL_WARN, ErrMsg("Trying to register unknown component type"));
      break;
    }
  }

  // Register systems
  std::string const systemsFile{ *Assets::AssetManager::GetInstance().GetConfigData<std::string>("Systems") };
  std::vector<std::pair<std::string, ECS::ComponentSignature>> const systems{ Serialization::DeserializeSystems(systemsFile) };

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

  // u probably wouldnt do this but prasanna went through this last week
  // move the object into the map since we don't need it anymore
  m_prefabs.insert(std::move(prefab));
}

GE::ECS::Entity ObjectFactory::SpawnPrefab(const std::string& key) const
{
  if (m_prefabs.find(key) == m_prefabs.end())
  {
    throw GE::Debug::Exception<ObjectFactory>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load prefab " + key));
  }

  ObjectData prefab = m_prefabs.at(key);
  Entity entity = CreateObject(prefab);
  if (IsBitSet(prefab.m_componentSignature, COMPONENT_TYPES::SPRITE_ANIM))
  {
    prefab.m_systemSignature[static_cast<unsigned>(SYSTEM_TYPES::SPRITE_ANIM)] = true;
  }
  return entity;
}

void ObjectFactory::CloneObject(ECS::Entity obj, const Math::dVec2& newPos)
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  Entity newObj = ecs.CreateEntity();
  CloneComponents(newObj, obj);
  Component::Transform* trans{ ecs.GetComponent<Component::Transform>(newObj) };
  if (trans) 
  {
    trans->m_pos = newPos;
  }
  ECS::SystemSignature sysSig{ GetObjectSystemSignature(obj) };
  if (ecs.GetComponentSignature(newObj)[static_cast<unsigned>(COMPONENT_TYPES::SPRITE_ANIM)])
  {
    sysSig[static_cast<unsigned>(SYSTEM_TYPES::SPRITE_ANIM)] = true;
  }
  RegisterObjectToSystems(newObj, sysSig);
}

bool ObjectFactory::LoadObjects() const
{
  try
  {
    for (auto const& value : m_objects)
    {
      CreateObject(value.second);
    }
  }
  catch (GE::Debug::IExceptionBase& e)
  {
    e.LogSource();
    e.Log();
    return false;
  }
  return true;
}

void ObjectFactory::ObjectJsonLoader(const std::string& json_path)
{
  GE::Serialization::ObjectGooStream ogs(json_path);
  ogs.Unload(m_objects);
}

// Reads objects from scene file and loads into map
void ObjectFactory::ObjectFactoryTest() {
  Serialization::ObjectGooStream ogs{ "Assets/Data/Scene.json" };
  if (ogs)
  {
    ogs.Unload(m_objects);
    LoadObjects();
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
  case SYSTEM_TYPES::ROOT_TRANSFORM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::RootTransformSystem>();
    RegisterComponentsToSystem<Systems::RootTransformSystem>(sig);
    break;
  default:
    throw Debug::Exception<ObjectFactory>(Debug::LEVEL_WARN, ErrMsg("Trying to register unknown system type"));
    break;
  }
}
