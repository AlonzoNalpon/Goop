#include <pch.h>
#include "ObjectFactory.h"

#include <Physics/PhysicsSystem.h>
#include <Physics/CollisionSystem.h>
#include <DraggableObject/DraggableObjectSystem.h>

#include "SerializeComponents.h"
#include "../Serialization/ObjectGooStream.h"
#include "../Serialization/PrefabGooStream.h"

using namespace GE::ObjectFactory;
using namespace GE::ECS;

void ObjectFactory::RegisterObject(GE::ECS::Entity object)
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  ecs.RegisterEntityToSystem<GE::Systems::PhysicsSystem>(object);
  //ecs.RegisterEntityToSystem<GE::Systems::CollisionSystem>(object);
  //ecs.RegisterEntityToSystem<GE::Systems::AdditionSystem>(object);

}

void ObjectFactory::RegisterPrefab(GE::ECS::Entity object, ECS::SystemSignature signature)
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
}

void RegisterComponents()
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };
  ecs.RegisterComponent<Transform>();
  ecs.RegisterComponent<Velocity>();
  ecs.RegisterComponent<GE::Gravity>();
  ecs.RegisterComponent<BoxCollider>();
  //ecs.RegisterComponent<Tween>();
  //ecs.RegisterComponent<Sprite>();
  //ecs.RegisterComponent<Model>();
  //ecs.RegisterComponent<Animation>();

  ecs.RegisterComponentToSystem<Transform, GE::Systems::PhysicsSystem>();   
  ecs.RegisterComponentToSystem<Velocity, GE::Systems::PhysicsSystem>();    
  ecs.RegisterComponentToSystem<GE::Gravity, GE::Systems::PhysicsSystem>();       //0000 0111
  ecs.RegisterComponentToSystem<BoxCollider, GE::Systems::CollisionSystem>();   //0000 1000

  //ecs.RegisterComponentToSystem<Tween, GE::Systems::PlayerControllerSystem>();  //0001 0000

}

GE::ECS::Entity ObjectFactory::CreateObject(ObjectData data)
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
      DeserializeComponent<GE::Gravity>(data.m_components[GE::ECS::COMPONENT_TYPES::GRAVITY]));
  }
  //=========================================================================================
  /*if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::SPRITE))
  {
    ecs.AddComponent(newData, COMPONENT_TYPES::SPRITE);

  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::MODEL))
  {
    ecs.AddComponent(newData, COMPONENT_TYPES::MODEL);

  }
  if (IsBitSet(data.m_componentSignature, COMPONENT_TYPES::ANIMATION))
  {
    ecs.AddComponent(newData, COMPONENT_TYPES::ANIMATION);

  }*/

  return newData;
}

void ObjectFactory::LoadPrefabsFromFile()
{
  // TODO: SHOULD GET FILE FROM Config.json "Prefabs" IN FUTURE; retrived from AssetManager
  const char* prefabsFile{ "Assets/Data/Prefabs/Prefabs.txt" };
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

GE::ECS::Entity ObjectFactory::SpawnPrefab(const std::string& key)
{
  if (m_prefabs.find(key) == m_prefabs.end())
  {
    // throw exception
  }
  PrefabData& prefab = m_prefabs[key];
  ObjectData object{ prefab.m_componentSignature, prefab.m_components };
  Entity entity = CreateObject(object);
  RegisterPrefab(entity, prefab.m_systemSignature);
  return entity;
}

int ObjectFactory::LoadObject()
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

void ObjectFactory::JoelTest()
{
  RegisterComponents();
  RegisterObject(CreateObject(m_objects["Player"]));
}



void ObjectFactory::ObjectJsonLoader(const std::string& json_path)
{
  GE::Serialization::ObjectGooStream ogs(json_path);
  ogs.Unload(m_objects);

  /*for (auto const& value2 : m_objects["Object2"].m_components.at(GE::ECS::COMPONENT_TYPES::TRANSFORM))
  {
    std::cout << value2.first << "|" << value2.second << std::endl;
  }*/
  
}

// Reads objects from file and loads into map
// prints contents of map to console
void ObjectFactory::ObjectFactoryTest() {
  std::map<std::string, ObjectData> m_objects;
  Serialization::ObjectGooStream ogs{ "Assets/Data/SERIALIZED.json" };
  ogs.Unload(m_objects);
}