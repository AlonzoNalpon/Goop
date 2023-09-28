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

#include <Physics/PhysicsSystem.h>
#include <Physics/CollisionSystem.h>
#include <DraggableObject/DraggableObjectSystem.h>
#include <Rendering/RenderingSystem.h>
#include <SpriteAnim/SpriteAnimSystem.h>

#include "SerializeComponents.h"
#include "../Serialization/ObjectGooStream.h"
#include "../Serialization/PrefabGooStream.h"
#include "../Systems/Rendering/RenderingSystem.h"

using namespace GE::ObjectFactory;
using namespace GE::ECS;

GE::ECS::SystemSignature ObjectFactory::GetObjectSystemSignature(GE::ECS::Entity obj) const
{
  GE::ECS::SystemSignature sig{};

  SetBitIfFound<Systems::PhysicsSystem>(obj, sig, ECS::SYSTEM_TYPES::PHYSICS);
  SetBitIfFound<Systems::DraggableObjectSystem>(obj, sig, ECS::SYSTEM_TYPES::DRAGGABLE_OBJECT);
  SetBitIfFound<Systems::RenderSystem>(obj, sig, ECS::SYSTEM_TYPES::RENDERING);
  SetBitIfFound<Systems::CollisionSystem>(obj, sig, ECS::SYSTEM_TYPES::COLLISION);
  SetBitIfFound<Systems::PlayerControllerSystem>(obj, sig, ECS::SYSTEM_TYPES::PLAYER_CONTROLLER);

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
}

void ObjectFactory::CloneComponents(GE::ECS::Entity destObj, GE::ECS::Entity srcObj) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  if (ecs.GetComponent<Component::Transform>(srcObj))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Transform>(srcObj));
  }
  if (ecs.GetComponent<Component::BoxCollider>(srcObj))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::BoxCollider>(srcObj));
  }
  if (ecs.GetComponent<Component::Velocity>(srcObj))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Velocity>(srcObj));
  }
  if (ecs.GetComponent<Component::Gravity>(srcObj))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Gravity>(srcObj));
  }
  if (ecs.GetComponent<Component::Sprite>(srcObj))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Sprite>(srcObj));
  }
  if (ecs.GetComponent<Component::SpriteAnim>(srcObj))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::SpriteAnim>(srcObj));
  }
  if (ecs.GetComponent<Component::Model>(srcObj))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Model>(srcObj));
  }
  /*if (ecs.GetComponent<Component::Tween>(srcObj))
  {
    ecs.AddComponent(destObj, *ecs.GetComponent<Component::Tween>(srcObj));
  }*/
}

void ObjectFactory::RegisterComponentsAndSystems() const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  // Register systems
  ecs.RegisterSystem<GE::Systems::PhysicsSystem>();
  ecs.RegisterSystem<GE::Systems::CollisionSystem>();
  ecs.RegisterSystem<GE::Systems::DraggableObjectSystem>();
  ecs.RegisterSystem<GE::Systems::PlayerControllerSystem>();
  ecs.RegisterSystem<GE::Systems::SpriteAnimSystem>();
  ecs.RegisterSystem<GE::Systems::RenderSystem>();

  // Register components in order of COMPONENT_TYPES enum
  ecs.RegisterComponent<Transform>();
  ecs.RegisterComponent<Velocity>();
  ecs.RegisterComponent<Gravity>();
  ecs.RegisterComponent<BoxCollider>();
  ecs.RegisterComponent<Tween>();
  ecs.RegisterComponent<Sprite>();
  ecs.RegisterComponent<SpriteAnim>();
  ecs.RegisterComponent<Model>();

  ecs.RegisterComponentToSystem<Transform, Systems::PhysicsSystem>();   
  ecs.RegisterComponentToSystem<Velocity, Systems::PhysicsSystem>();    
  ecs.RegisterComponentToSystem<Gravity, Systems::PhysicsSystem>();

  ecs.RegisterComponentToSystem<Transform, Systems::CollisionSystem>();
  ecs.RegisterComponentToSystem<BoxCollider, Systems::CollisionSystem>();

  ecs.RegisterComponentToSystem<Transform, Systems::DraggableObjectSystem>();
  ecs.RegisterComponentToSystem<BoxCollider, Systems::DraggableObjectSystem>();

  ecs.RegisterComponentToSystem<Model, Systems::RenderSystem>();
  ecs.RegisterComponentToSystem<Sprite, Systems::RenderSystem>();
  ecs.RegisterComponentToSystem<Transform, Systems::RenderSystem>();
  
  ecs.RegisterComponentToSystem<Tween, GE::Systems::PlayerControllerSystem>();

}

GE::ECS::Entity ObjectFactory::CreateObject(ObjectData data) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  Entity newData = ecs.CreateEntity();

  if (!newData)
  {
    // throw GE::Debug::Exception<Scene>(GE::Debug::LEVEL_CRITICAL, "Couldn't load object data.", ERRLG_FUNC, ERRLG_Line);
  }

  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::TRANSFORM))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Transform>(data.m_components[GE::ECS::COMPONENT_TYPES::TRANSFORM]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::BOXCOLLIDER))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::BoxCollider>(data.m_components[GE::ECS::COMPONENT_TYPES::BOXCOLLIDER]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::VELOCITY))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Velocity>(data.m_components[GE::ECS::COMPONENT_TYPES::VELOCITY]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::GRAVITY))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Gravity>(data.m_components[GE::ECS::COMPONENT_TYPES::GRAVITY]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SPRITE))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Sprite>(data.m_components[GE::ECS::COMPONENT_TYPES::SPRITE]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SPRITEANIM))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::SpriteAnim>(data.m_components[GE::ECS::COMPONENT_TYPES::SPRITEANIM]));
  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::MODEL))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Model>(data.m_components[GE::ECS::COMPONENT_TYPES::MODEL]));
  }
  /*if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::TWEEN))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Tween>(data.m_components[GE::ECS::COMPONENT_TYPES::TWEEN]));
  }*/

  return newData;
}

void ObjectFactory::LoadPrefabsFromFile()
{
  const char* prefabsFile{ AssetManager::AssetManager::GetInstance().GetConfigData<const char*>("Prefabs").value() };
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

void ObjectFactory::DeserializePrefab(const std::string& filepath)
{
  GE::Serialization::PrefabGooStream pgs{ filepath };
  if (!pgs)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + filepath);
  }

  std::pair<std::string, PrefabData> prefab;
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
    // throw exception
    std::cout << "ERROR";
  }
  PrefabData prefab = m_prefabs.at(key);
  ObjectData object{ prefab.m_componentSignature, prefab.m_components };
  Entity entity = CreateObject(object);
  if (IsBitSet(prefab.m_componentSignature, COMPONENT_TYPES::SPRITEANIM))
  {
    prefab.m_systemSignature[static_cast<unsigned>(SYSTEM_TYPES::SPRITE_ANIM)] = true;
  }
  RegisterObjectToSystems(entity, prefab.m_systemSignature);
  return entity;
}

void ObjectFactory::CloneObject(ECS::Entity obj, Math::dVec2&& newPos)
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  Entity newObj = ecs.CreateEntity();
  CloneComponents(newObj, obj);
  Component::Transform* trans{ ecs.GetComponent<Component::Transform>(newObj) };
  if (trans) 
  {
    trans->m_pos = newPos;
  }
  ECS::SystemSignature const sysSig{ GetObjectSystemSignature(obj) };
  RegisterObjectToSystems(newObj, sysSig);
}

int ObjectFactory::LoadObject() const
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
    return 1;
  }
  return 0;
}

void ObjectFactory::ObjectJsonLoader(const std::string& json_path)
{
  GE::Serialization::ObjectGooStream ogs(json_path);
  ogs.Unload(m_objects);
}

// Reads objects from file and loads into map
// prints contents of map to console
void ObjectFactory::ObjectFactoryTest() {
  std::map<std::string, ObjectData> m_objects;
  Serialization::ObjectGooStream ogs{ "Assets/Data/Scene.json" };
  ogs.Unload(m_objects);
}