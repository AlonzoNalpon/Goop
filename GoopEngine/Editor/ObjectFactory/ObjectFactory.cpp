#include <pch.h>
#include "ObjectFactory.h"

#include <Physics/PhysicsSystem.h>
#include <Physics/CollisionSystem.h>

#include "OFTransformData.h"
#include "../Serialization/ObjectGooStream.h"
using namespace GE::ObjectFactory;
using namespace GE::ECS;

void ObjectFactory::RegisterObject(GE::ECS::Entity object)
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  //if SYSTEM, REGISTER TO SYSTEM
  // I will need a system signature in order to register the entity into the relevant system
  ecs.RegisterEntityToSystem<GE::Systems::PhysicsSystem>(object);
  //ecs.RegisterEntityToSystem<GE::Systems::CollisionSystem>(object);
  //ecs.RegisterEntityToSystem<GE::Systems::AdditionSystem>(object);

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

  if (IsBitSet(data.m_signature, COMPONENT_TYPES::TRANSFORM))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Transform>(data.m_components[GE::ECS::COMPONENT_TYPES::TRANSFORM]));
  }
  if (IsBitSet(data.m_signature, COMPONENT_TYPES::BOXCOLLIDER))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::BoxCollider>(data.m_components[GE::ECS::COMPONENT_TYPES::BOXCOLLIDER]));
  }
  if (IsBitSet(data.m_signature, COMPONENT_TYPES::VELOCITY))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Component::Velocity>(data.m_components[GE::ECS::COMPONENT_TYPES::VELOCITY]));
  }
  if (IsBitSet(data.m_signature, COMPONENT_TYPES::GRAVITY))
  {
    ecs.AddComponent(newData,
      DeserializeComponent<GE::Gravity>(data.m_components[GE::ECS::COMPONENT_TYPES::GRAVITY]));
  }
  //=========================================================================================
  /*if (IsBitSet(data.m_signature, COMPONENT_TYPES::SPRITE))
  {
    ecs.AddComponent(newData, COMPONENT_TYPES::SPRITE);

  }
  if (IsBitSet(data.m_signature, COMPONENT_TYPES::MODEL))
  {
    ecs.AddComponent(newData, COMPONENT_TYPES::MODEL);

  }
  if (IsBitSet(data.m_signature, COMPONENT_TYPES::ANIMATION))
  {
    ecs.AddComponent(newData, COMPONENT_TYPES::ANIMATION);

  }*/

  return newData;
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

void ObjectFactory::ObjectJsonLoader(std::string json_path)
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
  Serialization::ObjectGooStream ogs{ "../Assets/AssetsToLoadTest/SERIALIZED.json" };
  ogs.Unload(m_objects);
  /*ogs.Reset();
  ogs.Read(m_objects);
  ogs.Unload("../Assets/AssetsToLoadTest/SERIALIZED.json");*/

  //// Create and fill up the first object
  //ObjectData object1{};
  //object1.m_signature.set(static_cast<size_t>(GE::ECS::COMPONENT_TYPES::ANIMATION), true);
  //object1.m_components[GE::ECS::COMPONENT_TYPES::ANIMATION]["Property1"] = "Value1";
  //object1.m_components[GE::ECS::COMPONENT_TYPES::BOXCOLLIDER]["pos_x"] = "0";
  //object1.m_components[GE::ECS::COMPONENT_TYPES::BOXCOLLIDER]["pos_y"] = "0";
  //object1.m_components[GE::ECS::COMPONENT_TYPES::BOXCOLLIDER]["width"] = "1";
  //object1.m_components[GE::ECS::COMPONENT_TYPES::BOXCOLLIDER]["height"] = "1";
  //m_objects["Object1"] = object1;

  //// Create and fill up the second object
  //ObjectData object2{};
  //object2.m_signature.set(static_cast<size_t>(GE::ECS::COMPONENT_TYPES::TRANSFORM), true);
  //object2.m_signature.set(static_cast<size_t>(GE::ECS::COMPONENT_TYPES::GRAVITY), true);
  //object2.m_components[GE::ECS::COMPONENT_TYPES::TRANSFORM]["pos_x"] = "39.2314";
  //object2.m_components[GE::ECS::COMPONENT_TYPES::TRANSFORM]["pos_y"] = "5.12315";
  //object2.m_components[GE::ECS::COMPONENT_TYPES::TRANSFORM]["scale_x"] = "39.2314";
  //object2.m_components[GE::ECS::COMPONENT_TYPES::TRANSFORM]["scale_y"] = "5.12315";
  //object2.m_components[GE::ECS::COMPONENT_TYPES::TRANSFORM]["rot"] = "5.12315";

  //object2.m_components[GE::ECS::COMPONENT_TYPES::GRAVITY]["grav_x"] = "0";
  //object2.m_components[GE::ECS::COMPONENT_TYPES::GRAVITY]["grav_y"] = "9.807";
  //m_objects["Object2"] = object2;
  //std::cout << "\n\n\n" << object2.m_signature.to_ulong() << "\n\n\n";

  //// Create and fill up the third object
  //ObjectData object3{};
  //object3.m_signature.set(static_cast<size_t>(GE::ECS::COMPONENT_TYPES::VELOCITY), true);
  //object3.m_signature.set(static_cast<size_t>(GE::ECS::COMPONENT_TYPES::ANIMATION), true);
  //object3.m_components[GE::ECS::COMPONENT_TYPES::VELOCITY]["acc_x"] = "0";
  //object3.m_components[GE::ECS::COMPONENT_TYPES::VELOCITY]["acc_y"] = "0";
  //object3.m_components[GE::ECS::COMPONENT_TYPES::VELOCITY]["vel_x"] = "0";
  //object3.m_components[GE::ECS::COMPONENT_TYPES::VELOCITY]["vel_y"] = "0";

  //object3.m_components[GE::ECS::COMPONENT_TYPES::ANIMATION]["Property6"] = "Value6";
  //object3.m_components[GE::ECS::COMPONENT_TYPES::ANIMATION]["Property4446"] = "Value2346";
  //object3.m_components[GE::ECS::COMPONENT_TYPES::ANIMATION]["Property466"] = "Value3436";
  //m_objects["Object3"] = object3;

  //GE::Serialization::ObjectGooStream ogs{"../Assets/AssetsToLoadTest/SERIALIZED.json"};
  //ogs.Reset();
  //ogs.Read(m_objects);
  //ogs.Unload("../Assets/AssetsToLoadTest/SERIALIZED.json");


  //std::cout << "=========OBJECT FACTORY SERIALIZE TEST=========\n";
  //for (const auto& pair : m_objects) {
  //  const std::string& objectName = pair.first;
  //  const ObjectData& objectData = pair.second;

  //  std::cout << "Object Name: " << objectName << std::endl;
  //  std::cout << "Signature: " << objectData.m_signature.to_ulong() << std::endl;

  //  for (const auto& componentPair : objectData.m_components) {
  //    GE::ECS::COMPONENT_TYPES componentType = componentPair.first;
  //    const MemberMap& memberMap = componentPair.second;

  //    std::cout << "Component Type: " << ECS::componentsToString.at(componentType) << std::endl;

  //    for (const auto& memberPair : memberMap) {
  //      const std::string& memberName = memberPair.first;
  //      const std::string& memberValue = memberPair.second;

  //      std::cout << "Member Name: " << memberName << ", Member Value: " << memberValue << std::endl;
  //    }
  //  }
  //}
  //std::cout << "===============================================\n";
}
